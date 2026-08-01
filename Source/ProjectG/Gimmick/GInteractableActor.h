#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/GInteractable.h"
#include "GInteractableActor.generated.h"

class UGInteractionCondition;
class UGInteractionAction;
class UGInteractionActionPipeline;
class USphereComponent;
class UAnimMontage;
class AGCharacter;

UCLASS()
class PROJECTG_API AGInteractableActor : public AActor, public IGInteractable
{
	GENERATED_BODY()

public:
	AGInteractableActor();

	virtual FName GetID() const { return ID; }
	virtual int32 GetPriority() const override { return Priority; }
	virtual float GetHoldDuration() const override { return HoldDuration; }
	virtual EGInteractionState GetInteractionState(AActor* TargetActor) override;
	virtual void OnInteractStarted(AActor* TargetActor) override;
	virtual bool CanInteract(AActor* TargetActor) override;
	virtual void Interact(AActor* TargetActor) override;
	virtual void InternalInteract(AActor* TargetActor) override;

	FTransform GetInteractPointTransform() const;
	UAnimMontage* GetInteractMontage() const;
	FName GetInteractNotifyName() const { return InteractNotifyName; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void RequestAsyncLoad();
	void OnConditionsLoaded();
	void RequestAsyncLoadActions();
	void OnActionsLoaded();
	void OnInteractionCompleted();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float HoldDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TArray<TSoftClassPtr<UGInteractionCondition>> ConditionClassPtrs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Actions")
	TArray<TSoftClassPtr<UGInteractionAction>> ActionClassPtrs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Actions")
	TSoftObjectPtr<UAnimMontage> InteractMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Actions")
	FName InteractNotifyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Actions")
	TObjectPtr<USceneComponent> InteractPoint;

protected:
	UPROPERTY(Transient)
	EGInteractionState InteractionState;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGInteractionCondition>> Conditions;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGInteractionAction>> Actions;

	UPROPERTY(Transient)
	float StartTimestamp;

	UPROPERTY(Transient)
	TObjectPtr<UGInteractionActionPipeline> Pipeline;

	UPROPERTY(Transient)
	TWeakObjectPtr<AGCharacter> InteractingCharacter;
};
