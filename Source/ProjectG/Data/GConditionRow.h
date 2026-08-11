#pragma once

#include "CoreMinimal.h"
#include "Data/GGameEnums.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "GConditionRow.generated.h"

USTRUCT(BlueprintType)
struct FGConditionRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems);
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName);

	FName GetID() const { return ID; }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGConditionType ConditionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName IDParam;  // 맵ID, 아이템ID 등
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 IntParam;   // 레벨, 아이템 수량 등

private:
	FName ID;
};
