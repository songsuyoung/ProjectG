#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GInteractable.generated.h"

enum class EGInteractionState : uint8;
UINTERFACE(MinimalAPI)
class UGInteractable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTG_API IGInteractable
{
	GENERATED_BODY()

public:
	virtual FName GetID() const = 0;
	virtual int32 GetPriority() const = 0;
	virtual float GetHoldDuration() const = 0;
	virtual EGInteractionState GetInteractionState(AActor* TargetActor) = 0;
	virtual void OnInteractStarted(AActor* TargetActor) = 0;
	virtual bool CanInteract(AActor* TargetActor) = 0;
	virtual void Interact(AActor* TargetActor) = 0;
	virtual void InternalInteract(AActor* TargetActor) = 0;
};
