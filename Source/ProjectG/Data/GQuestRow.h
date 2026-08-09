#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GQuestRow.generated.h"

class UGCondition;
enum class EGQuestState	: uint8;

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
	TArray<TSubclassOf<UGCondition>> Conditions;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName PrerequisiteQID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextQID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EGQuestState, FName> DialogueID;

	
private:
	FName ID;
};
