#pragma once

#include "CoreMinimal.h"
#include "Data/Condition/GCondition.h"
#include "Interface/GInteractable.h"
#include "GInteractionCondition.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class PROJECTG_API UGInteractionCondition : public UGCondition
{
	GENERATED_BODY()

public:
	virtual void Init(IGInteractable* Interact);

protected:
	TWeakInterfacePtr<IGInteractable> Interactable;
};
