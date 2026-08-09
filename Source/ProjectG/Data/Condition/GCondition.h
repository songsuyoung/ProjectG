#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GCondition.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class PROJECTG_API UGCondition : public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied(AActor* TargetActor) { return true; }
};
