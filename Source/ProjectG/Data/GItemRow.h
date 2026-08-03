#pragma once

#include "CoreMinimal.h"
#include "GItemRow.generated.h"

class UTexture2D;
class AGInteractableActor;
USTRUCT(BlueprintType)
struct FGItemRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems);
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName);
	
	FName GetID() { return ID; }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Desc;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> IconImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AGInteractableActor> ItemActor;
	
private:
	
	FName ID;
};