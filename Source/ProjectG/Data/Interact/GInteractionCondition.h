#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interface/GInteractable.h"
#include "GInteractionCondition.generated.h"

UCLASS(Blueprintable)
class PROJECTG_API UGInteractionCondition : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(IGInteractable* Interact);
	virtual bool IsSatisfied(AActor* TargetActor) { return true; }
	virtual void Apply(AActor* TargetActor) {}

protected:
	TWeakInterfacePtr<IGInteractable> Interactable;
};
