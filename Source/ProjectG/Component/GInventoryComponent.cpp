#include "Component/GInventoryComponent.h"

#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GInteractionPromptRow.h"
#include "Data/GItemRow.h"
#include "Data/GMessage.h"
#include "System/GDataManager.h"
#include "System/GEventManager.h"

UGInventoryComponent::UGInventoryComponent()
{

}

bool UGInventoryComponent::CanAcquire(FName ItemID)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);
	
	FGItemRow* ItemRow = DataManager->GetDataTableRow<FGItemRow>(EGDataTableType::Item, ItemID);

	if (nullptr == ItemRow)
	{
		return false;
	}
	int32* ItemCount = InventorySlots.Find(ItemRow->GetID());
	
	if (ItemCount == nullptr)
	{
		return true; 
	}
	
	return *ItemCount < ItemRow->MaxCount;
}

void UGInventoryComponent::Acquire(FName PromptID)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	FGInteractionPromptRow* InteractionPromptRow = DataManager->GetDataTableRow<FGInteractionPromptRow>(EGDataTableType::InteractionPrompt, PromptID);
	
	if (nullptr == InteractionPromptRow)
	{
		return;
	}
	
	FGItemRow* ItemRow = DataManager->GetDataTableRow<FGItemRow>(EGDataTableType::Item, InteractionPromptRow->ItemID);

	if (nullptr == ItemRow)
	{
		return;
	}
	
	FName AcquiredItemID = ItemRow->GetID();
	auto& ItemValue = InventorySlots.FindOrAdd(AcquiredItemID);
	ItemValue += 1;

	FGItemMessage Message(AcquiredItemID, ItemValue);
	GEVENT_BROADCAST(this, GGameplayTags::EventTag_Item_Acquired, Message);
	GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Quest);
}

void UGInventoryComponent::UseItem(FName ItemName, int32 Count)
{
	int32* ItemCount = InventorySlots.Find(ItemName);
	
	if (ItemCount == nullptr)
	{
		// 아이템 사용 불가능 함.
		return;
	}
	
	*ItemCount = (Count - 1) > 0? *ItemCount - 1 : 0;

	FGItemMessage Message(ItemName, *ItemCount);
	GEVENT_BROADCAST(this, GGameplayTags::EventTag_Item_Removed, Message);
}

bool UGInventoryComponent::CanUseItem(FName ItemName, int32 Count)
{
	int32* ItemCount = InventorySlots.Find(ItemName);
	
	if (ItemCount == nullptr)
	{
		// 아이템 사용 불가능 함.
		return false;
	}
	
	return *ItemCount >= Count;
}
