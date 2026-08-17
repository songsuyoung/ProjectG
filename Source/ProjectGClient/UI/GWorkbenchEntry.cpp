#include "GWorkbenchEntry.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/GItemRow.h"
#include "Engine/AssetManager.h"
#include "System/GDataManager.h"

void UGWorkbenchEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UGItemEntry* ItemEntry = static_cast<UGItemEntry*>(ListItemObject);
	
	if (false == IsValid(ItemEntry))
	{
		return;
	}
	
	UGDataManager* DataManager = UGDataManager::Get(this);
	
	check(DataManager);
	
	FGItemRow* ItemRow = DataManager->GetDataTableRow<FGItemRow>(EGDataTableType::Item, ItemEntry->ItemID);
	
	if (nullptr == ItemRow)
	{
		return;
	}
	if (IsValid(TextBlock_Name))
	{
		TextBlock_Name->SetText(ItemRow->Desc);
	}
	
	if (ItemEntry->IconImage.IsValid())
	{
		Image_Icon->SetBrushFromTexture(ItemEntry->IconImage.Get());
		Image_Icon->SetVisibility(ESlateVisibility::Visible);
	
		return;
	}
	
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableHandle = StreamableManager.RequestAsyncLoad(
		ItemEntry->IconImage.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ThisClass::OnIconLoaded)
	);
}

void UGWorkbenchEntry::OnIconLoaded()
{
	if (false == IsValid(Image_Icon))
	{
		return;
	}

	if (StreamableHandle.IsValid() && StreamableHandle->HasLoadCompleted())
	{
		UTexture2D* Texture = Cast<UTexture2D>(StreamableHandle->GetLoadedAsset());
		if (IsValid(Texture))
		{
			Image_Icon->SetBrushFromTexture(Texture);
			Image_Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}