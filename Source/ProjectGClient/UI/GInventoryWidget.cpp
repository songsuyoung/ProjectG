#include "UI/GInventoryWidget.h"

#include "GInventoryOptions.h"
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
		OwnerInventoryComponent = Character->GetInventoryComponent();
		if (OwnerInventoryComponent.IsValid())
		{
			for (const TPair<FName, int32>& InventorySlot : OwnerInventoryComponent->GetInventorySlots())
			{
				AddInventoryUI(InventorySlot.Key, InventorySlot.Value);
			}
		}
	}
	else
	{
		TileView_Inventory->SetListItems(Entries);
	}

	TileView_Inventory->OnItemClicked().AddUObject(this, &ThisClass::OnItemClicked);
	
	// Event.Item 부모 태그로 등록 → Acquired/Removed 모두 수신
	GEVENT_ADD(this, GGameplayTags::EventTag_Item, this);
	
	if (IsValid(InventoryOptions))
	{
		InventoryOptions->OnClickedOption.BindUObject(this, &ThisClass::OnOptionClicked);
	}
}

void UGInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (false == IsValid(TileView_Inventory))
	{
		return;
	}
	
	TileView_Inventory->ClearListItems();
	
	if (IsValid(InventoryOptions))
	{
		InventoryOptions->OnClickedOption.Unbind();
		InventoryOptions->SetVisibility(ESlateVisibility::Collapsed);
	}
	
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

UGItemEntry* UGInventoryWidget::CreateItemEntry(FName ItemID, const FGItemRow* ItemRow, int32 ItemCount)
{
	UGItemEntry* NewEntry = NewObject<UGItemEntry>(this);
	NewEntry->ItemID = ItemID;
	NewEntry->IconImage = ItemRow->IconImage;
	NewEntry->Count = ItemCount;
	NewEntry->ItemType = ItemRow->Type;
	
	return NewEntry;
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
		
		Entries[Index] = CreateItemEntry(ItemID, ItemRow, ItemCount);
		TileView_Inventory->SetListItems(Entries);
		return;
	}

	// 빈 슬롯 없음 → 3개 확장 후 첫 칸에 배치
	for (int32 Index = 0; Index < 3; Index++)
	{
		Entries.Add(NewObject<UGItemEmptyEntry>(this));
	}
	
	Entries[Entries.Num() - 3] = CreateItemEntry(ItemID, ItemRow, ItemCount);;
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

void UGInventoryWidget::OnItemClicked(UObject* ItemObject)
{
	if (false == IsValid(TileView_Inventory) || false == IsValid(InventoryOptions))
	{
		return;
	}
	
	UGItemEntry* ItemEntry = Cast<UGItemEntry>(ItemObject);
	if (IsValid(ItemEntry))
	{
		if (EGItemType::Material == ItemEntry->ItemType)
		{
			InventoryOptions->SetVisibility(ESlateVisibility::Collapsed);
			// 사용하지 못함. 재화용도임.
			return;
		}
	}
	
	UGInventoryEntry* EntryWidget = TileView_Inventory->GetEntryWidgetFromItem<UGInventoryEntry>(ItemObject);
	
	if (IsValid(EntryWidget))
	{
		// EntryWidget의 Geometry 값을 가져옴
		FVector2D MousePos;                                                                                                                                                  
		GetOwningPlayer()->GetMousePosition(MousePos.X, MousePos.Y);                                                                                                                                                                          

		FGeometry ParentGeometry = GetCachedGeometry();
		FVector2D LocalPos = ParentGeometry.AbsoluteToLocal(MousePos);

		InventoryOptions->SetRenderTranslation(LocalPos);

		InventoryOptions->SetVisibility(ESlateVisibility::Visible);
	}
	
	SelectedObject = ItemObject;
}

void UGInventoryWidget::OnOptionClicked(EGOptionType OptionType)
{
	if (false == OwnerInventoryComponent.IsValid())
	{
		return;
	}
	
	UGItemEntry* ItemEntry = Cast<UGItemEntry>(SelectedObject);
	
	if (false == IsValid(ItemEntry))
	{
		return;	
	}
	
	switch (OptionType)
	{
	case EGOptionType::Use:	
		OwnerInventoryComponent->UseItem(ItemEntry->ItemID, ItemEntry->Count);
		break;
	case EGOptionType::Keep:
		break;
	}
}
