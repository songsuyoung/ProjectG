#pragma once

#include "CoreMinimal.h"
#include "GInteractionCondition.h"
#include "GInteractionCondition_ItemRequirement.generated.h"

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
class PROJECTG_API UGInteractionCondition_ItemRequirement : public UGInteractionCondition
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied(AActor* TargetActor) override;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition")
	FGItemRequirement Item;
};
