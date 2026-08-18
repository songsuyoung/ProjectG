#include "UI/GInventoryWidget.h"

#include "Base/GTileView.h"
#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"
#include "Data/GGameEnums.h"
#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GItemRow.h"
#include "Data/GMessage.h"
#include "System/GDataManager.h"
#include "System/GEventManager.h"
#include "System/GUIManager.h"
#include "UI/GInventoryEntry.h"

UGInventoryWidget::UGInventoryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InitialSlotCount(25)
{
}

void UGInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == IsValid(TileView_Inventory))
	{
		return;
	}

	Entries.Reset();

	for (int32 Index = 0; Index < InitialSlotCount; Index++)
	{
		Entries.Add(NewObject<UGItemEmptyEntry>(this));
	}

	AGCharacter* Character = Cast<AGCharacter>(GetOwningPlayerPawn());
	if (IsValid(Character))
	{
		UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
		if (IsValid(InventoryComponent))
		{
			for (const TPair<FName, int32>& InventorySlot : InventoryComponent->GetInventorySlots())
			{
				AddInventoryUI(InventorySlot.Key, InventorySlot.Value);
			}
		}
	}
	else
	{
		TileView_Inventory->SetListItems(Entries);
	}

	// Event.Item 부모 태그로 등록 → Acquired/Removed 모두 수신
	GEVENT_ADD(this, GGameplayTags::EventTag_Item, this);
}

void UGInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (false == IsValid(TileView_Inventory))
	{
		return;
	}
	
	TileView_Inventory->ClearListItems();
	
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Item, this);
}

bool UGInventoryWidget::NativeOnHandleBackAction()
{
	if (UGUIManager* UIManager = UGUIManager::Get(this))
	{
		UIManager->CloseWindow(this);
		return true;
	}

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
	if (ItemCount <= 0)
	{
		return;
	}
	
	for (int32 Index = 0; Index < Entries.Num(); Index++)
	{
		UGItemEntry* Entry = Cast<UGItemEntry>(Entries[Index]);
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

	for (int32 Index = 0; Index < Entries.Num(); Index++)
	{
		if (Entries[Index]->GetClass() != UGItemEmptyEntry::StaticClass())
		{
			continue;
		}

		UGItemEntry* NewEntry = NewObject<UGItemEntry>(this);
		NewEntry->ItemID = ItemID;
		NewEntry->IconImage = ItemRow->IconImage;
		NewEntry->Count = ItemCount;
		Entries[Index] = NewEntry;
		TileView_Inventory->SetListItems(Entries);
		return;
	}

	// 빈 슬롯 없음 → 3개 확장 후 첫 칸에 배치
	for (int32 Index = 0; Index < 3; Index++)
	{
		Entries.Add(NewObject<UGItemEmptyEntry>(this));
	}

	UGItemEntry* NewEntry = NewObject<UGItemEntry>(this);
	NewEntry->ItemID = ItemID;
	NewEntry->IconImage = ItemRow->IconImage;
	NewEntry->Count = ItemCount;
	Entries[Entries.Num() - 3] = NewEntry;
	TileView_Inventory->SetListItems(Entries);
}

void UGInventoryWidget::UseInventoryUI(FName ItemID, int32 ItemCount)
{
	for (int32 Index = 0; Index < Entries.Num(); Index++)
	{
		UGItemEntry* Entry = Cast<UGItemEntry>(Entries[Index]);
		if (false == IsValid(Entry) || Entry->ItemID != ItemID)
		{
			continue;
		}

		if (ItemCount <= 0)
		{
			Entries[Index] = NewObject<UGItemEmptyEntry>(this);
			TileView_Inventory->SetListItems(Entries);
			return;
		}

		Entry->Count = ItemCount;
		TileView_Inventory->RequestRefresh();
		return;
	}
}
