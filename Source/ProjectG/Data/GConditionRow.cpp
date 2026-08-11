#include "GConditionRow.h"

void FGConditionRow::OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems)
{
	FTableRowBase::OnPostDataImport(InDataTable, InRowName, OutCollectedImportProblems);

	ID = InRowName;
}

void FGConditionRow::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	FTableRowBase::OnDataTableChanged(InDataTable, InRowName);

	ID = InRowName;
}
