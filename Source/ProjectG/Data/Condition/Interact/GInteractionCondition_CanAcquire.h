#pragma once

#include "CoreMinimal.h"
#include "GInteractionCondition.h"
#include "GInteractionCondition_CanAcquire.generated.h"

UCLASS()
class PROJECTG_API UGInteractionCondition_CanAcquire : public UGInteractionCondition
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied(AActor* TargetActor, FGConditionRow* Condition) override;

protected:
	bool IsSatisfied(AActor* TargetActor, FName ItemID);
};
