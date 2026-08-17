#pragma once

#include "CoreMinimal.h"
#include "GWorkbenchRow.generated.h"

USTRUCT(BlueprintType)
struct FGWorkbenchIngredient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Count = 1;
};

USTRUCT(BlueprintType)
struct FGWorkbenchRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems);
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName);

	FName GetID() const { return ID; }

	// 결과물 아이템 ID (Item DataTable RowName)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ResultItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ResultCount = 1;

	// 재료 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGWorkbenchIngredient> Ingredients;

private:

	FName ID;
};
