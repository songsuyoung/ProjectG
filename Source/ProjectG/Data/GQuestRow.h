#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GQuestRow.generated.h"

USTRUCT(BlueprintType)
struct FGQuestRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems);
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName);

	FName GetID() const { return ID; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NPCID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName PreDialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SuccessDialogueID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextQuestID;

private:
	FName ID;
};
