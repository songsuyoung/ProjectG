#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/GInteractable.h"
#include "GInteractableActor.generated.h"

class USphereComponent;
UCLASS()
class PROJECTG_API AGInteractableActor : public AActor, public IGInteractable
{
	GENERATED_BODY()
	
public:	
	AGInteractableActor();

	virtual int32 GetPriority() const override { return Priority; }
	virtual EGInteractionState GetInteractionState() const override { return InteractionState; }
	virtual bool CanInteract(AActor* TargetActor) const override { return true; }
	virtual void Interact(AActor* TargetActor) override;
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	EGInteractionState InteractionState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	int32 Priority;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
};
