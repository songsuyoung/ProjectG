#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GNPCRow.generated.h"

USTRUCT(BlueprintType)
struct FGNPCRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems);
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName);

	FName GetID() const { return ID; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DefaultDialogueID;
	
private:
	FName ID;
};
