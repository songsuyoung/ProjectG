#pragma once

#include "CoreMinimal.h"
#include "Character/GBaseCharacter.h"
#include "Interface/GInteractable.h"
#include "GNPCCharacter.generated.h"

class UGInteractionCondition;
class UGInteractionActionComponent;
class UGDialogueComponent;

UCLASS()
class PROJECTG_API AGNPCCharacter : public AGBaseCharacter, public IGInteractable
{
	GENERATED_BODY()

public:
	AGNPCCharacter();

	FName GetID() { return NPCID; }
	virtual FName GetInteractionID() const override { return InteractionID; }
	virtual int32 GetPriority() const override { return InteractionPriority; }
	virtual float GetHoldDuration() const override { return HoldDuraction; }
	virtual void InternalInteract(AActor* TargetActor) override;
	virtual bool IsRepeatable() const override { return true; }

	const TArray<TObjectPtr<UGInteractionCondition>>& GetConditions() const override { return Conditions; }

protected:
	virtual void BeginPlay() override;
	virtual FGInteractionSharedState& GetInteractionSharedState() override { return SharedState; }
	virtual UGInteractionActionComponent* GetInteractionActionComponent() override { return InteractionActionComponent; }

protected:
	UPROPERTY(EditAnywhere, Category = "NPC")
	FName NPCID;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	FName InteractionID;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	int32 InteractionPriority;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float HoldDuraction;

	UPROPERTY(EditAnywhere, Instanced, Category = "Interaction")
	TArray<TObjectPtr<UGInteractionCondition>> Conditions;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGInteractionActionComponent> InteractionActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dialogue, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGDialogueComponent> DialogueComponent;

protected:
	FGInteractionSharedState SharedState;
};
