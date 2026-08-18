#include "Data/Interact/Action/GInteractionAction_Talk.h"

#include "Character/GCharacter.h"
#include "Character/GNPCCharacter.h"
#include "Component/GQuestComponent.h"
#include "Data/GDialogueRow.h"
#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GMessage.h"
#include "Data/GNPCRow.h"
#include "System/GDataManager.h"
#include "System/GConditionManager.h"
#include "System/GDialogueManager.h"
#include "System/GEventManager.h"
#include "System/GUIManagerBase.h"

void UGInteractionAction_Talk::Execute(AActor* OwnerActor, AActor* TargetActor)
{
	if (false == IsValid(OwnerActor) || false == IsValid(TargetActor))
	{
		Finish();
		return;
	}

	QuestComponentRef = TargetActor->FindComponentByClass<UGQuestComponent>();
	CharacterRef = Cast<AGCharacter>(TargetActor);
	const AGNPCCharacter* NPCCharacter = Cast<AGNPCCharacter>(OwnerActor);

	if (false == QuestComponentRef.IsValid() || false == CharacterRef.IsValid() || false == IsValid(NPCCharacter))
	{
		Finish();
		return;
	}

	CachedNPCID = NPCCharacter->GetID();

	UGDialogueManager* Dialogue = UGDialogueManager::Get(this);
	if (false == IsValid(Dialogue))
	{
		Finish();
		return;
	}

	Dialogue->OnDialogueEnded.AddUObject(this, &ThisClass::OnDialogueFinished);
	GEVENT_ADD(this, GGameplayTags::EventTag_Dialogue_SelectChoice, this);

	CachedDialogueID = ResolveDialogueID(CachedNPCID, TargetActor);

	UGUIManagerBase* UIManager = UGUIManagerBase::Get(this);
	check(UIManager);
	UIManager->OpenWindow(GGameplayTags::UITag_Window_Dialogue, GGameplayTags::UITag_Layout_Popup);

	Dialogue->StartDialogue(CachedDialogueID);
}

void UGInteractionAction_Talk::Finish()
{
	Super::Finish();

	UGDialogueManager* Dialogue = UGDialogueManager::Get(this);
	if (false == IsValid(Dialogue))
	{
		return;
	}

	GEVENT_REMOVE(this, GGameplayTags::EventTag_Dialogue_SelectChoice, this);
	Dialogue->OnDialogueEnded.RemoveAll(this);
}

void UGInteractionAction_Talk::OnMessage(FGameplayTag Tag, FGMessage* Message)
{
	if (nullptr == Message)
	{
		return;
	}

	FGChoiceMessage* ChoiceMsg = static_cast<FGChoiceMessage*>(Message);
	if (ChoiceMsg->ChoiceTag.MatchesTag(GGameplayTags::UITag_Window))
	{
		UGUIManagerBase* UIManager = UGUIManagerBase::Get(this);
		check(UIManager);
		UIManager->OpenWindow(ChoiceMsg->ChoiceTag, GGameplayTags::UITag_Layout_Popup);
	}
}

void UGInteractionAction_Talk::OnDialogueFinished(EGDialogueEndReason EndReason)
{
	if (EndReason == EGDialogueEndReason::Completed)
	{
		if (QuestComponentRef.IsValid())
		{
			QuestComponentRef->OnNPCDialogueCompleted(CachedNPCID);
		}

		if (CharacterRef.IsValid() && false == CachedDialogueID.IsNone())
		{
			CharacterRef->AddCompletedDialogue(CachedDialogueID);
		}

	}

	QuestComponentRef = nullptr;
	CharacterRef = nullptr;
	CachedNPCID = NAME_None;
	CachedDialogueID = NAME_None;
	Finish();
}

FName UGInteractionAction_Talk::ResolveDialogueID(FName NPCID, AActor* TargetActor)
{
	// 1. 퀘스트에서 지정된 대화
	QuestComponentRef->CheckQuest(NPCID);
	FName DID = QuestComponentRef->GetDialogueForNPC(NPCID);
	if (false == DID.IsNone())
	{
		return DID;
	}

	// 2. Condition 조건을 만족하는 대화
	DID = CheckNextDialogueID(NPCID, TargetActor);
	if (false == DID.IsNone())
	{
		return DID;
	}

	// 3. NPC 기본 대화
	UGDataManager* DataManager = UGDataManager::Get(this);
	FGNPCRow* NPCRow = DataManager->GetDataTableRow<FGNPCRow>(EGDataTableType::NPC, NPCID);
	if (nullptr != NPCRow)
	{
		return NPCRow->DefaultDialogueID;
	}

	return FName();
}

FName UGInteractionAction_Talk::CheckNextDialogueID(FName NPCID, AActor* TargetActor)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	UDataTable* DialogueDT = DataManager->GetDataTable(EGDataTableType::Dialogue);
	if (false == IsValid(DialogueDT))
	{
		return FName();
	}

	TArray<FGDialogueRow*> Rows;
	DialogueDT->GetAllRows(TEXT(""), Rows);

	UGConditionManager* ConditionManager = UGConditionManager::Get(this);
	check(ConditionManager);

	for (FGDialogueRow* Row : Rows)
	{
		if (Row->NPCID != NPCID || Row->ConditionIDs.IsEmpty())
		{
			continue;
		}

		if (CharacterRef.IsValid() && CharacterRef->HasCompletedDialogue(Row->GetID()))
		{
			continue;
		}

		bool bIsSatisfied = true;
		for (const FName& ConditionID : Row->ConditionIDs)
		{
			if (false == ConditionManager->IsSatisfied(ConditionID, TargetActor))
			{
				bIsSatisfied = false;
				break;
			}
		}

		if (bIsSatisfied)
		{
			return Row->GetID();
		}
	}

	return FName();
}
