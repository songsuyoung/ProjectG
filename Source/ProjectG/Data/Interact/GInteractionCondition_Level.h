#pragma once

#include "CoreMinimal.h"
#include "GInteractionCondition.h"
#include "GInteractionCondition_Level.generated.h"

UCLASS()
class PROJECTG_API UGInteractionCondition_Level : public UGInteractionCondition
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied(AActor* TargetActor);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition | Level")
	int32 Level;
};
