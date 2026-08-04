#include "UI/GInventoryWidget.h"

#include "Base/GTileView.h"
#include "Data/GGameEnums.h"
#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GItemRow.h"
#include "Data/GMessage.h"
#include "System/GDataManager.h"
#include "System/GEventManager.h"
#include "UI/GInventoryEntry.h"

void UGInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == IsValid(TileView_Inventory))
	{
		return;
	}

	Entries.Reset();

	for (int32 i = 0; i < InitialSlotCount; i++)
	{
		UGItemEmptyEntry* EmptyEntry = NewObject<UGItemEmptyEntry>(this);
		Entries.Add(EmptyEntry);
	}

	TileView_Inventory->SetListItems(Entries);

	// Event.Item 부모 태그로 등록 → Acquired/Removed 모두 수신
	GEVENT_ADD(this, GGameplayTags::EventTag_Item, this);
}

void UGInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Item, this);
}

bool UGInventoryWidget::NativeOnHandleBackAction()
{
	DeactivateWidget();
	return Super::NativeOnHandleBackAction();
}

void UGInventoryWidget::OnMessage(FGameplayTag Tag, FGMessage* Message)
{
	FGItemMessage* ItemMessage = static_cast<FGItemMessage*>(Message);
	if (nullptr == ItemMessage)
	{
		return;
	}

	if (Tag == GGameplayTags::EventTag_Item_Acquired)
	{
		AddInventoryUI(ItemMessage->ItemID, ItemMessage->ItemCount);
	}
	else if (Tag == GGameplayTags::EventTag_Item_Removed)
	{
		UseInventoryUI(ItemMessage->ItemID, ItemMessage->ItemCount);
	}
}

void UGInventoryWidget::AddInventoryUI(FName ItemID, int32 ItemCount)
{
	for (int32 i = 0; i < Entries.Num(); i++)
	{
		UGItemEntry* Entry = Cast<UGItemEntry>(Entries[i]);
		if (IsValid(Entry) && Entry->ItemID == ItemID)
		{
			Entry->Count = ItemCount;
			TileView_Inventory->RequestRefresh();
			return;
		}
	}

	UGDataManager* DataManager = UGDataManager::Get(this);
	if (false == IsValid(DataManager))
	{
		return;
	}

	FGItemRow* ItemRow = DataManager->GetDataTableRow<FGItemRow>(EGDataTableType::Item, ItemID);
	if (nullptr == ItemRow)
	{
		return;
	}

	for (int32 i = 0; i < Entries.Num(); i++)
	{
		if (Entries[i]->GetClass() != UGItemEmptyEntry::StaticClass())
		{
			continue;
		}

		UGItemEntry* NewEntry = NewObject<UGItemEntry>(this);
		NewEntry->ItemID = ItemID;
		NewEntry->IconImage = ItemRow->IconImage;
		NewEntry->Count = ItemCount;

		Entries[i] = NewEntry;
		TileView_Inventory->SetListItems(Entries);
		return;
	}
}

void UGInventoryWidget::UseInventoryUI(FName ItemID, int32 ItemCount)
{
	for (int32 i = 0; i < Entries.Num(); i++)
	{
		UGItemEntry* Entry = Cast<UGItemEntry>(Entries[i]);
		if (false == IsValid(Entry) || Entry->ItemID != ItemID)
		{
			continue;
		}

		if (ItemCount <= 0)
		{
			Entries[i] = NewObject<UGItemEmptyEntry>(this);
			TileView_Inventory->SetListItems(Entries);
			return;
		}

		Entry->Count = ItemCount;
		TileView_Inventory->RequestRefresh();
		return;
	}
}
