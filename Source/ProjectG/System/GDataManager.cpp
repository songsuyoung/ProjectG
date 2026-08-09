#include "GDataManager.h"

#include "GGameInstance.h"
#include "Data/GDataAsset.h"

// UE
#include "GEventManager.h"
#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


UGDataManager* UGDataManager::Get(const UObject* Object)
{
    UWorld* World = Object->GetWorld();

    check(World);

    UGGameInstance* GameInstance = Cast<UGGameInstance>(World->GetGameInstance());

    check(GameInstance);

    return GameInstance->GetDataManager();
}

void UGDataManager::Initialize()
{
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    Streamable.RequestAsyncLoad(DataAssetClass.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &ThisClass::LoadDataAsset));
}

void UGDataManager::LoadDataAsset()
{
    DataAsset = DataAssetClass.Get();
    GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Open_DataTable);
}

TArray<UDataTable*> UGDataManager::GetAllDataTable()
{
    TArray<UDataTable*> Results;

    for (EGDataTableType Type : TEnumRange<EGDataTableType>())
    {
        UDataTable* DataTable  = GetDataTable(Type);

        if (IsValid(DataTable))
        {
            Results.Add(DataTable);
        }
    }
    return Results;
}

UDataTable* UGDataManager::GetDataTable(const EGDataTableType& DataType)
{
    if (false == IsValid(DataAsset))
    {
        return nullptr;
    }

    return DataAsset->GetTable(DataType);
}

UDataTable* UGDataManager::GetDataTable(const FName& DataType)
{
    return nullptr;
}
