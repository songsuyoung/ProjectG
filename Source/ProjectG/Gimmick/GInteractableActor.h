#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/GInteractable.h"
#include "GInteractableActor.generated.h"

class UGInteractionActionComponent;
class USphereComponent;
class UAnimMontage;
class AGCharacter;

UCLASS()
class PROJECTG_API AGInteractableActor : public AActor, public IGInteractable
{
	GENERATED_BODY()

public:
	AGInteractableActor();

	virtual FName GetInteractionID() const override { return ID; }
	virtual int32 GetPriority() const override { return Priority; }
	virtual float GetHoldDuration() const override { return HoldDuration; }
	virtual void InternalInteract(AActor* TargetActor) override {}

	FTransform GetInteractPointTransform() const;

protected:
	virtual FGInteractionSharedState& GetInteractionSharedState() override { return SharedState; }
	virtual UGInteractionActionComponent* GetInteractionActionComponent() override { return InteractionActionComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float HoldDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Actions")
	TObjectPtr<USceneComponent> InteractPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UGInteractionActionComponent> InteractionActionComponent;

protected:
	
	FGInteractionSharedState SharedState;
};
