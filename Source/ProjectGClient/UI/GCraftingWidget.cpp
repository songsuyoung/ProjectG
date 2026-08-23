#include "GCraftingWidget.h"

#include "GInventoryEntry.h"
#include "Base/GTileView.h"
#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"
#include "Data/GItemRow.h"
#include "Data/GWorkbenchRow.h"
#include "System/GDataManager.h"
#include "System/GUIManager.h"
#include "UI/Base/GPrimaryWidget.h"

UGCraftingWidget::UGCraftingWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MaxSlots(9)
{
}

void UGCraftingWidget::OnMessage(FGameplayTag Tag, FGMessage* Message)
{
}

void UGCraftingWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	Init();
}

void UGCraftingWidget::Init()
{
	if (false == WorkbenchIngredientEntry.IsEmpty())
	{
		return;
	}

	if (false == IsValid(TileView_Workbench))
	{
		return;
	}
	
	UGDataManager* DataManager = UGDataManager::Get(this);
	
	check(DataManager);
	
	UDataTable* DataTable = DataManager->GetDataTable(EGDataTableType::Workbench);
	
	if (IsValid(DataTable))
	{
		TArray<FGWorkbenchRow*> WorkbenchRows;
		DataTable->GetAllRows(TEXT(""), WorkbenchRows);
		
		for (int Index = 0; Index < WorkbenchRows.Num(); Index++)
		{
			FGItemRow* ItemRow = DataManager->GetDataTableRow<FGItemRow>(EGDataTableType::Item, WorkbenchRows[Index]->ResultItemID);
			
			if (nullptr == ItemRow)
			{
				continue;
			}
			
			UGItemEntry* ItemEntry = NewObject<UGItemEntry>(this);
			ItemEntry->ItemID = ItemRow->GetID();
			ItemEntry->IconImage = ItemRow->IconImage;
			ItemEntry->ItemName = ItemRow->Name;
			
			TileView_Workbench->AddItem(ItemEntry);
			WorkbenchIngredientEntry.Add(WorkbenchRows[Index]->ResultItemID, { WorkbenchRows[Index]->Ingredients });
		}
	}

	TileView_Workbench->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnWorkbenchItemSelected);
}

void UGCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AGCharacter* Character = Cast<AGCharacter>(GetOwningPlayerPawn());
	
	if (IsValid(Character))
	{
		InventoryComponent = Character->GetInventoryComponent();
	}

	if (IsValid(Button_Maker))
	{
		Button_Maker->OnClicked().AddUObject(this, &ThisClass::OnClicked);
	}
}

void UGCraftingWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (IsValid(Button_Maker))
	{
		Button_Maker->OnClicked().RemoveAll(this);
	}
	
	if (IsValid(TileView_Ingredients))
	{
		TileView_Ingredients->ClearListItems();
	}
	
	InventoryComponent = nullptr;
}

void UGCraftingWidget::OnClicked()
{
	if (false == InventoryComponent.IsValid())
	{
		return;
	}
	
	if (false == SelectedItem.IsValid())
	{
		return;
	}
	
	UGItemEntry* ItemEntry = Cast<UGItemEntry>(SelectedItem);
	if (false == IsValid(ItemEntry))
	{
		return;
	}
	
	// CurrentItem에 대해 InventoryComponent 전달. 
	// 사용가능 여부 확인후 사용
	// 그리고 새로운 Item 전달 
	FGWorkbenchIngredientEntry* Entry = WorkbenchIngredientEntry.Find(ItemEntry->ItemID);
	if (nullptr == Entry)
	{
		return;
	}
	
	for (const FGWorkbenchIngredient& Ingredient : Entry->Ingredients)
	{
		bool bCanMake = InventoryComponent->CanUseItem(Ingredient.ItemID, Ingredient.Count);
		
		if (false == bCanMake)
		{
			return;
		}
	}
	
	for (const FGWorkbenchIngredient& Ingredient : Entry->Ingredients)
	{
		InventoryComponent->UseItem(Ingredient.ItemID, Ingredient.Count);
	}
	
	InventoryComponent->Acquire(ItemEntry->ItemID);
	OnWorkbenchItemSelected(SelectedItem.Get());
}

void UGCraftingWidget::OnWorkbenchItemSelected(UObject* Item)
{
	if (false == IsValid(TileView_Ingredients))
	{
		return;
	}
	
	if (false == InventoryComponent.IsValid())
	{
		return;
	}
	
	UGItemEntry* ItemEntry = Cast<UGItemEntry>(Item);
	if (false == IsValid(ItemEntry))
	{
		return;
	}

	FGWorkbenchIngredientEntry* Entry = WorkbenchIngredientEntry.Find(ItemEntry->ItemID);
	if (nullptr == Entry)
	{
		return;
	}

	TileView_Ingredients->ClearListItems();

	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);
	
	for (const FGWorkbenchIngredient& Ingredient : Entry->Ingredients)
	{
		FGItemRow* ItemRow = DataManager->GetDataTableRow<FGItemRow>(EGDataTableType::Item, Ingredient.ItemID);
		if (nullptr == ItemRow)
		{
			continue;
		}

		// 플레이어의 InventoryComponent로부터 ItemID가 Count만큼 있는지 확인한다.
		// 만약, ItemID가 Count만큼 존재하지 않는다면, 잠금 처리해둔다.
		UGItemEntry* IngredientEntry = NewObject<UGItemEntry>(this);
		IngredientEntry->ItemID = Ingredient.ItemID;
		IngredientEntry->IconImage = ItemRow->IconImage;
		IngredientEntry->Count = Ingredient.Count - InventoryComponent->GetItemCount(Ingredient.ItemID);
		IngredientEntry->bLocked = (false == InventoryComponent->CanUseItem(Ingredient.ItemID, Ingredient.Count));

		TileView_Ingredients->AddItem(IngredientEntry);
	}

	int32 FilledCount = TileView_Ingredients->GetNumItems();
	for (int32 Index = FilledCount; Index < MaxSlots; Index++)
	{
		TileView_Ingredients->AddItem(NewObject<UGItemEmptyEntry>(this));
	}
	
	SelectedItem = ItemEntry;
}

bool UGCraftingWidget::NativeOnHandleBackAction()
{
	if (UGUIManager* UIManager = UGUIManager::Get(this))
	{
		UIManager->CloseWindow(this);
		return true;
	}
	
	return Super::NativeOnHandleBackAction();
}
