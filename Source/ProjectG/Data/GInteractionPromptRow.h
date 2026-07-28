#pragma once

#include "CoreMinimal.h"
#include "GInteractionPromptRow.generated.h"

USTRUCT(BlueprintType)
struct FGInteractionPromptRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems);
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName);
	
	FName GetID() { return ID; }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText PromptText;
	
private:
	
	FName ID;
};