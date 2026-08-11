#include "GCondition_ItemRequirement.h"

#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"
#include "Data/GConditionRow.h"

bool UGCondition_ItemRequirement::IsSatisfied(AActor* TargetActor, FName ItemID, int32 ItemCount)
{
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);
	if (false == IsValid(Character))
	{
		return false;
	}

	UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
	if (false == IsValid(InventoryComponent))
	{
		return false;
	}

	return InventoryComponent->CanUseItem(ItemID, ItemCount);
}

bool UGCondition_ItemRequirement::IsSatisfied(AActor* TargetActor, FGConditionRow* Condition)
{
	return IsSatisfied(TargetActor, Condition->IDParam, Condition->IntParam);
}
