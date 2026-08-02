#include "Component/GInventoryComponent.h"

#include "Data/GGameMacro.h"
#include "Data/GInteractionPromptRow.h"
#include "Data/GItemRow.h"
#include "Data/GMessage.h"
#include "System/GDataManager.h"
#include "System/GEventManager.h"

UGInventoryComponent::UGInventoryComponent()
{

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

	FGItemMessage Message(EGMessageType::ItemAcquired, AcquiredItemID, ItemValue);
	GEVENT_MESSAGE_NOTIFY_MSG(this, EGMessageType::ItemAcquired, Message);
}

void UGInventoryComponent::UseItem(FName ItemName, int32 Count)
{
	int32* ItemCount = InventorySlots.Find(ItemName);
	
	if (ItemCount == nullptr)
	{
		// 아이템 사용 불가능 함.
		return;
	}
	
	*ItemCount -= Count;

	FGItemMessage Message(EGMessageType::ItemRemoved, ItemName, *ItemCount);
	GEVENT_MESSAGE_NOTIFY_MSG(this, EGMessageType::ItemRemoved, Message);
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
