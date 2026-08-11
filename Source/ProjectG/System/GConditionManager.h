#pragma once

#include "CoreMinimal.h"
#include "Data/GGameEnums.h"
#include "UObject/NoExportTypes.h"
#include "GConditionManager.generated.h"

class UGCondition;

UCLASS(Blueprintable)
class PROJECTG_API UGConditionManager : public UObject
{
	GENERATED_BODY()

public:
	static UGConditionManager* Get(const UObject* Context);
	bool IsSatisfied(const FName& ConditionID, AActor* RelatedActor = nullptr);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Condition")
	TMap<EGConditionType, TSubclassOf<UGCondition>> Conditions;
};
