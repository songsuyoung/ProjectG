#pragma once

#include "CoreMinimal.h"
#include "GCondition.h"
#include "GCondition_ItemRequirement.generated.h"

USTRUCT(BlueprintType)
struct FGItemRequirement
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Count;
};

UCLASS()
class PROJECTG_API UGCondition_ItemRequirement : public UGCondition
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied(AActor* TargetActor, FGConditionRow* Condition) override;

protected:
	bool IsSatisfied(AActor* TargetActor, FName ItemID, int32 ItemCount);
	
};
