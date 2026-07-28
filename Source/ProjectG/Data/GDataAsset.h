#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/GGameEnums.h"
#include "GDataAsset.generated.h"

UCLASS()
class PROJECTG_API UGDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UDataTable* GetTable(const EGDataTableType& DataType) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Data Tables")
	TMap<EGDataTableType, TObjectPtr<UDataTable>> DataTableMap;
};
