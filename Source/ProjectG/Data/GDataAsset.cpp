#include "GDataAsset.h"

// UE
#include "Engine/DataTable.h"


UDataTable* UGDataAsset::GetTable(const EGDataTableType& DataType) const
{
	if (DataTableMap.Contains(DataType))
	{
		return DataTableMap[DataType];
	}

	return nullptr;
}
