#pragma once

#include "CoreMinimal.h"
#include "Data/GGameEnums.h"
#include "GInteractionAction.h"
#include "GInteractionAction_Talk.generated.h"
class UGQuestComponent;
class AGCharacter;

UCLASS(Blueprintable)
class PROJECTG_API UGInteractionAction_Talk : public UGInteractionAction
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
	virtual void Finish() override;
protected:
	void OnDialogueFinished(EGDialogueEndReason EndReason);
	
	FName CheckNextDialogueID(FName NPCID, AActor* TargetActor);
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UGQuestComponent> QuestComponentRef;

	UPROPERTY(Transient)
	TWeakObjectPtr<AGCharacter> CharacterRef;

	UPROPERTY(Transient)
	FName CachedNPCID;

	UPROPERTY(Transient)
	FName CachedDialogueID;
};
