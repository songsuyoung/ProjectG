#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/GGameEnums.h"
#include "GInteractable.generated.h"

class UGCondition;
class UGInteractionActionComponent;
class AGCharacter;

struct FGInteractionSharedState
{
	EGInteractionState State = EGInteractionState::Available;
	float StartTimestamp = 0.f;
	TWeakObjectPtr<AGCharacter> InteractingCharacter;
};

UINTERFACE(MinimalAPI)
class UGInteractable : public UInterface
{
	GENERATED_BODY()
};

class PROJECTG_API IGInteractable
{
	GENERATED_BODY()

public:
	virtual FName GetInteractionID() const = 0;
	virtual int32 GetPriority() const = 0;
	virtual float GetHoldDuration() const = 0;
	virtual EGInteractionState GetInteractionState(AActor* TargetActor);
	virtual void OnInteractStarted(AActor* TargetActor);
	virtual bool CanInteract(AActor* TargetActor);
	virtual void Interact(AActor* TargetActor);
	virtual void InternalInteract(AActor* TargetActor) = 0;
	virtual bool IsRepeatable() const { return false; }
	virtual UGInteractionActionComponent* GetInteractionActionComponent() = 0;
protected:
	virtual FGInteractionSharedState& GetInteractionSharedState() = 0;
};
