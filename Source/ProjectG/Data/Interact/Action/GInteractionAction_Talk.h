#pragma once

#include "CoreMinimal.h"
#include "Data/GGameEnums.h"
#include "Interface/GMessageReceiver.h"
#include "GInteractionAction.h"
#include "GInteractionAction_Talk.generated.h"
class UGQuestComponent;
class AGCharacter;

UCLASS(Blueprintable)
class PROJECTG_API UGInteractionAction_Talk : public UGInteractionAction, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
	virtual void Finish() override;

protected:
	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;

	void OnDialogueFinished(EGDialogueEndReason EndReason);

	FName ResolveDialogueID(FName NPCID, AActor* TargetActor);
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
