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
	virtual int32 GetPriority() const = 0;
	virtual EGInteractionState GetInteractionState() const = 0;
	virtual bool CanInteract(AActor* Instigator) const = 0;
	virtual void Interact(AActor* Instigator) = 0;
};
