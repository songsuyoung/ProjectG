#pragma once

#include "CoreMinimal.h"
#include "GCondition.h"
#include "GCondition_Level.generated.h"

UCLASS()
class PROJECTG_API UGCondition_Level : public UGCondition
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied(AActor* TargetActor, FGConditionRow* Condition) override;
	
protected:
	bool IsSatisfied(AActor* TargetActor, int32 Level);
	
};
