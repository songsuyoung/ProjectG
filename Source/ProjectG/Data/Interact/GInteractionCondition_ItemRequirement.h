#pragma once

#include "CoreMinimal.h"
#include "GInteractionCondition.h"
#include "GInteractionCondition_ItemRequirement.generated.h"

UCLASS()
class PROJECTG_API UGInteractionCondition_ItemRequirement : public UGInteractionCondition
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied(AActor* TargetActor);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition | Level")
	FName Item;
};
