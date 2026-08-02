#include "UI/GInventoryEntry.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "Engine/Texture2D.h"

void UGInventoryEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UGItemEntry* ItemEntry = Cast<UGItemEntry>(ListItemObject);

	if (false == IsValid(Image_Icon))
	{
		return;
	}

	if (false == IsValid(ItemEntry) || false == IsValid(TextBlock_Count) || ItemEntry->IconImage.IsNull())
	{
		Image_Icon->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FString Str = FString::Printf(TEXT("%d"), ItemEntry->Count);
	TextBlock_Count->SetText(FText::FromString(Str));
	
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

void UGInventoryEntry::OnIconLoaded()
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
