#include "Component/GInventoryComponent.h"

#include "Data/GInteractionPromptRow.h"
#include "Data/GItemRow.h"
#include "System/GDataManager.h"

UGInventoryComponent::UGInventoryComponent()
{

}

void UGInventoryComponent::Acquire(FName ItemName)
{
	// 실제로는 데이터 테이블에 있는 데이터 인지를 확인한다.
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);
	
	FGInteractionPromptRow* InteractionPromptRow = DataManager->GetDataTableRow<FGInteractionPromptRow>(EGDataTableType::InteractionPrompt, ItemName);
	
	if (nullptr == InteractionPromptRow)
	{
		return;
	}
	
	FGItemRow* ItemRow = DataManager->GetDataTableRow<FGItemRow>(EGDataTableType::Item, InteractionPromptRow->ItemID);

	if (nullptr == ItemRow)
	{
		return;
	}
	auto& ItemValue = InventorySlots.FindOrAdd(ItemRow->GetID());
	// 데이터가 실제로 있다면 하나 더해준다.
	ItemValue += 1;
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
