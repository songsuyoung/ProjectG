#include "System/GDialogueManager.h"

#include "GDataManager.h"
#include "GEventManager.h"
#include "GGameInstance.h"
#include "Data/GDialogueRow.h"
#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GMessage.h"

UGDialogueManager* UGDialogueManager::Get(const UObject* Context)
{
	UWorld* World = Context->GetWorld();

	check(World);

	UGGameInstance* GameInstance = Cast<UGGameInstance>(World->GetGameInstance());

	check(GameInstance);

	return GameInstance->GetDialogueManager();
}

void UGDialogueManager::StartDialogue(const FName& DID)
{
	GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Dialogue_Toggle);
	ShowNode(DID);
}

bool UGDialogueManager::NextDialogue()
{
	if (CachedNextID.IsNone())
	{
		return false;
	}
	
	ShowNode(CachedNextID);
	return true;
}

void UGDialogueManager::ShowNode(FName NodeID)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	FGDialogueRow* Node = DataManager->GetDataTableRow<FGDialogueRow>(EGDataTableType::Dialogue, NodeID);
	if (nullptr == Node)
	{
		EndDialogue(EGDialogueEndReason::Completed);
		return;
	}
	
	CurrentNodeID = NodeID;
	CachedNextID = Node->NextID;
	
	FGDialogueMessage Msg({Node->NPCID, Node->Body, Node->Choices});
	GEVENT_BROADCAST(this, GGameplayTags::EventTag_Dialogue_Node, Msg);
}

void UGDialogueManager::SelectChoice(const FGDialogueChoice& Choice)
{
	if (false == Choice.NextID.IsNone())
	{
		ShowNode(Choice.NextID);
	}
	else
	{
		EndDialogue(EGDialogueEndReason::Completed);
	}
	
	GEVENT_BROADCAST_EMPTY(this, Choice.ChoiceTag);
}

void UGDialogueManager::EndDialogue(EGDialogueEndReason EndReason)
{
	CurrentNodeID = NAME_None;
	CachedNextID = NAME_None;
	
	GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Dialogue_Toggle);
	OnDialogueEnded.Broadcast(EndReason);
}
