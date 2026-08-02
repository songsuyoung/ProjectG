#pragma once

#include "CoreMinimal.h"
#include "GInteractionAction.h"
#include "GInteractionAction_Spawn.generated.h"

class AGInteractableActor;

UCLASS(Blueprintable)
class PROJECTG_API UGInteractionAction_Spawn : public UGInteractionAction
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;

private:
	void OnClassLoaded(TSoftClassPtr<AGInteractableActor> SoftClass);

protected:

	UPROPERTY(EditAnywhere, Category = "Interaction|Spawn")
	FName SpawnID;

	UPROPERTY(EditAnywhere, Category = "Interaction|Spawn")
	float SpawnZOffset = 50.f;

private:

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> OwnerActorRef;
};
