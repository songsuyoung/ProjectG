#include "Data/Interact/Action/GInteractionAction_Talk.h"

#include "Character/GNPCCharacter.h"
#include "Component/GDialogueComponent.h"
#include "Component/GQuestComponent.h"
#include "Interface/GInteractable.h"

void UGInteractionAction_Talk::Execute(AActor* OwnerActor, AActor* TargetActor)
{
	if (false == IsValid(TargetActor))
	{
		Finish();
		return;
	}

	UGDialogueComponent* Dialogue = OwnerActor->FindComponentByClass<UGDialogueComponent>();
	if (false == IsValid(Dialogue))
	{
		Finish();
		return;
	}

	FName QuestID = NAME_None;
	FName QuestDialogueID = NAME_None;
	bool bIsSuccessDialogue = false;

	UGQuestComponent* QuestComp = TargetActor->FindComponentByClass<UGQuestComponent>();
	AGNPCCharacter* NPCCharacter = Cast<AGNPCCharacter>(OwnerActor);

	if (nullptr != QuestComp && nullptr != NPCCharacter)
	{
		const FName NPCID = NPCCharacter->GetID();

		// ReadyToComplete 우선 (성공 대사)
		QuestDialogueID = QuestComp->GetSuccessDialogueForNPC(NPCID, QuestID);
		if (false == QuestDialogueID.IsNone())
		{
			bIsSuccessDialogue = true;
		}
		else
		{
			// Available (제안 대사 + 선택지)
			QuestDialogueID = QuestComp->GetPreDialogueForNPC(NPCID, QuestID);
		}
	}

	// 성공 대사 종료 시 CompleteQuest 호출
	// AcceptQuest는 선택지 선택 시 EventTag_Quest_Accept 이벤트로 처리됨
	Dialogue->OnDialogueEnded.BindWeakLambda(this, [this, QuestComp, QuestID, bIsSuccessDialogue]()
	{
		if (bIsSuccessDialogue && IsValid(QuestComp) && false == QuestID.IsNone())
		{
			QuestComp->CompleteQuest(QuestID);
		}
		Finish();
	});

	if (false == QuestDialogueID.IsNone())
	{
		Dialogue->StartDialogue(QuestDialogueID);
	}
	else
	{
		Dialogue->StartDialogue(TargetActor);
	}
}
