#include "Data/Interact/Action/GInteractionAction_Talk.h"

#include "Character/GNPCCharacter.h"
#include "Component/GQuestComponent.h"
#include "Data/GNPCRow.h"
#include "System/GDataManager.h"
#include "System/GDialogueManager.h"

void UGInteractionAction_Talk::Execute(AActor* OwnerActor, AActor* TargetActor)
{
	if (false == IsValid(OwnerActor) || false == IsValid(TargetActor))
	{
		Finish();
		return;
	}

	QuestComponentRef = TargetActor->FindComponentByClass<UGQuestComponent>();
	const AGNPCCharacter* NPCCharacter = Cast<AGNPCCharacter>(OwnerActor);
	
	if (false == QuestComponentRef.IsValid() || false == IsValid(NPCCharacter))
	{
		Finish();
		return;
	}
	CachedNPCID = NPCCharacter->GetID();
	QuestComponentRef->CheckQuest(CachedNPCID);
	FName DID = QuestComponentRef->GetDialogueForNPC(CachedNPCID);
	
	UGDialogueManager* Dialogue = UGDialogueManager::Get(this);
	if (false == IsValid(Dialogue))
	{
		Finish();
		return;
	}
	
	Dialogue->OnDialogueEnded.AddUObject(this, &ThisClass::OnDialogueFinished);
	
	if (DID.IsNone())
	{
		UGDataManager* DataManager = UGDataManager::Get(this);
		FGNPCRow* NPCRow = DataManager->GetDataTableRow<FGNPCRow>(EGDataTableType::NPC, NPCCharacter->GetID());
		
		if (nullptr != NPCRow)
		{
			DID = NPCRow->DefaultDialogueID;
		}
	}
	
	Dialogue->StartDialogue(DID);
}

void UGInteractionAction_Talk::Finish()
{
	Super::Finish();
	
	UGDialogueManager* Dialogue = UGDialogueManager::Get(this);
	if (false == IsValid(Dialogue))
	{
		Finish();
		return;
	}
	Dialogue->OnDialogueEnded.RemoveAll(this);
}

void UGInteractionAction_Talk::OnDialogueFinished(EGDialogueEndReason EndReason)
{
	if (EndReason == EGDialogueEndReason::Completed)
	{
		if (QuestComponentRef.IsValid())
		{
			QuestComponentRef->OnNPCDialogueCompleted(CachedNPCID);
		}
	}

	QuestComponentRef = nullptr;
	CachedNPCID = NAME_None;
	Finish();
}
