#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/GGameEnums.h"
#include "GDataManager.generated.h"

class UGDataAsset;

UCLASS(Blueprintable)
class PROJECTG_API UGDataManager : public UObject
{
	GENERATED_BODY()

public:

	static UGDataManager* Get(UObject* Object);

	void Initialize();
public:
	TArray<UDataTable*> GetAllDataTable();
	UDataTable* GetDataTable(const EGDataTableType& DataType);
	UDataTable* GetDataTable(const FName& DataType);

	template<typename T>
	T* GetDataTableRow(const EGDataTableType& DataType, const FName& ID);

	template<typename T>
	T* GetDataTableRow(const EGDataTableType& DataType, int32 ID);

protected:

	void LoadDataAsset();

protected:
	UPROPERTY(EditAnywhere, Category = "DataTables")
	TSoftObjectPtr<UGDataAsset> DataAssetClass;

protected:

	UPROPERTY(Transient)
	TObjectPtr<UGDataAsset> DataAsset;
};

template<typename T>
T* UGDataManager::GetDataTableRow(const EGDataTableType& DataType, const FName& ID)
{
	UDataTable* DataTable = GetDataTable(DataType);

	if (IsValid(DataTable))
	{
		return DataTable->FindRow<T>(ID, FString());
	}

	return nullptr;
}

template<typename T>
T* UGDataManager::GetDataTableRow(const EGDataTableType& DataType, int32 ID)
{
	FString StrID = FString::Printf(TEXT("%d"), ID);

	const FName& IDName = FName(StrID);

	return GetDataTableRow(DataType, IDName);
}
