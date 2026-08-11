#include "Data/Interact/Action/GInteractionAction_Talk.h"

#include "Character/GCharacter.h"
#include "Character/GNPCCharacter.h"
#include "Component/GQuestComponent.h"
#include "Data/GDialogueRow.h"
#include "Data/GNPCRow.h"
#include "System/GDataManager.h"
#include "System/GConditionManager.h"
#include "System/GDialogueManager.h"

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
	
	// 현재 가능한 퀘스트가 있는가 먼저 확인필요
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
	
	// 없다면,
	if (DID.IsNone())
	{
		// 대화의 Condition조건을 만족하는 대화가 있는가 찾아본다.
		DID = CheckNextDialogueID(CachedNPCID, TargetActor);

		if (DID.IsNone())
		{
			// 다음 할 수 있는 대화도 없다면, 기본 대화를 실시한다.
			UGDataManager* DataManager = UGDataManager::Get(this);
			FGNPCRow* NPCRow = DataManager->GetDataTableRow<FGNPCRow>(EGDataTableType::NPC, NPCCharacter->GetID());

			if (nullptr != NPCRow)
			{
				DID = NPCRow->DefaultDialogueID;
			}
		}
	}

	CachedDialogueID = DID;
	Dialogue->StartDialogue(DID);
}

void UGInteractionAction_Talk::Finish()
{
	Super::Finish();
	
	UGDialogueManager* Dialogue = UGDialogueManager::Get(this);
	if (false == IsValid(Dialogue))
	{
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
