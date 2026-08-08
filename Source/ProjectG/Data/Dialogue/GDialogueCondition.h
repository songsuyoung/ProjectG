#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GDialogueCondition.generated.h"

UCLASS(Abstract, EditInlineNew)
class PROJECTG_API UGDialogueCondition : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied(AActor* TargetActor) { return true; }
};
