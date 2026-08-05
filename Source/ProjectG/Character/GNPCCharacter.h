#pragma once

#include "CoreMinimal.h"
#include "Character/GBaseCharacter.h"
#include "Interface/GInteractable.h"
#include "GNPCCharacter.generated.h"

class UGInteractionCondition;
class UGInteractionActionComponent;

UCLASS()
class PROJECTG_API AGNPCCharacter : public AGBaseCharacter, public IGInteractable
{
	GENERATED_BODY()

public:
	AGNPCCharacter();

	virtual FName GetID() const override { return InteractionID; }
	virtual int32 GetPriority() const override { return InteractionPriority; }
	virtual float GetHoldDuration() const override { return HoldDuraction; }
	virtual void InternalInteract(AActor* TargetActor) override;

	const TArray<TObjectPtr<UGInteractionCondition>>& GetConditions() const override { return Conditions; }

protected:
	virtual void BeginPlay() override;
	virtual FGInteractionSharedState& GetInteractionSharedState() override { return SharedState; }
	virtual UGInteractionActionComponent* GetInteractionActionComponent() override { return InteractionActionComponent; }

protected:
	UPROPERTY(EditAnywhere, Category = "Interaction|ID")
	FName InteractionID;

	UPROPERTY(EditAnywhere, Category = "Interaction|ID")
	int32 InteractionPriority;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float HoldDuraction;

	UPROPERTY(EditAnywhere, Instanced, Category = "Interaction")
	TArray<TObjectPtr<UGInteractionCondition>> Conditions;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGInteractionActionComponent> InteractionActionComponent;

protected:
	FGInteractionSharedState SharedState;
};
