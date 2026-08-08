#include "Component/GDialogueComponent.h"

#include "Data/Dialogue/GDialogueCondition.h"
#include "Data/GGameEnums.h"
#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GMessage.h"
#include "GameFramework/Character.h"
#include "System/GDataManager.h"
#include "System/GEventManager.h"


void UGDialogueComponent::StartDialogue(AActor* TargetActor)
{
	StartDialogue(GetCurrentDialogueID(TargetActor));
}

void UGDialogueComponent::StartDialogue(FName RootDialogueID)
{
	GEVENT_ADD(this, GGameplayTags::EventTag_Dialogue_Advance, this);
	GEVENT_ADD(this, GGameplayTags::EventTag_Dialogue_Cancel, this);
	GEVENT_ADD(this, GGameplayTags::EventTag_Dialogue_SelectChoice, this);

	GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Dialogue_Toggle);

	ShowNode(RootDialogueID);
}

void UGDialogueComponent::OnMessage(FGameplayTag Tag, FGMessage* Message)
{
	if (Tag == GGameplayTags::EventTag_Dialogue_Advance)
	{
		AdvanceDialogue();
	}
	else if (Tag == GGameplayTags::EventTag_Dialogue_Cancel)
	{
		EndDialogue();
	}
	else if (Tag == GGameplayTags::EventTag_Dialogue_SelectChoice)
	{
		FGDialogueChoiceMessage* ChoiceMsg = static_cast<FGDialogueChoiceMessage*>(Message);
		if (nullptr != ChoiceMsg)
		{
			SelectChoice(ChoiceMsg->ChoiceIndex);
		}
	}
}

void UGDialogueComponent::AdvanceDialogue()
{
	AdvanceDialogue(NAME_None);
}

void UGDialogueComponent::AdvanceDialogue(FName NextID)
{
	if (NextID == NAME_None)
	{
		UGDataManager* DataManager = UGDataManager::Get(this);
		check(DataManager);

		FGDialogueRow* Node = DataManager->GetDataTableRow<FGDialogueRow>(EGDataTableType::Dialogue, CurrentNodeID);
		if (nullptr == Node)
		{
			return;
		}

		// 선택지가 있으면 스페이스바로 넘기지 않음
		if (false == Node->Choices.IsEmpty())
		{
			return;
		}

		NextID = Node->NextID;
	}

	if (NextID == NAME_None)
	{
		return;
	}

	ShowNode(NextID);
}

void UGDialogueComponent::EndDialogue()
{
	CurrentNodeID = NAME_None;
	CachedNextID = NAME_None;

	GEVENT_REMOVE(this, GGameplayTags::EventTag_Dialogue_Advance, this);
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Dialogue_Cancel, this);
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Dialogue_SelectChoice, this);

	GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Dialogue_Toggle);
	OnDialogueEnded.ExecuteIfBound();
}

FName UGDialogueComponent::GetCurrentDialogueID(AActor* TargetActor) const
{
	for (const FGDialogueConditionEntry& Cond : Conditions)
	{
		if (false == IsValid(Cond.Condition))
		{
			continue;
		}

		if (Cond.Condition->IsSatisfied(TargetActor))
		{
			return Cond.RootDialogueID;
		}
	}

	return DefaultDialogueID;
}

void UGDialogueComponent::SelectChoice(int32 ChoiceIndex)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	FGDialogueRow* Node = DataManager->GetDataTableRow<FGDialogueRow>(EGDataTableType::Dialogue, CurrentNodeID);
	if (nullptr == Node || !Node->Choices.IsValidIndex(ChoiceIndex))
	{
		return;
	}

	const FGDialogueChoice& Choice = Node->Choices[ChoiceIndex];

	if (false == Choice.QuestID.IsNone())
	{
		FGQuestMessage QuestMsg(Choice.QuestID);
		GEVENT_BROADCAST(this, GGameplayTags::EventTag_Quest_Accept, QuestMsg);
	}

	if (false == Choice.NextID.IsNone())
	{
		ShowNode(Choice.NextID);
	}
	else
	{
		EndDialogue();
	}
}

void UGDialogueComponent::ShowNode(FName NodeID)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	FGDialogueRow* Node = DataManager->GetDataTableRow<FGDialogueRow>(EGDataTableType::Dialogue, NodeID);
	if (nullptr == Node)
	{
		return;
	}

	CurrentNodeID = NodeID;
	CachedNextID = Node->Choices.IsEmpty() ? Node->NextID : NAME_None;

	FGDialogueMessage Msg(*Node);
	GEVENT_BROADCAST(this, GGameplayTags::EventTag_Dialogue_Node, Msg);
}
