#include "GCondition_ItemRequirement.h"

#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"

bool UGCondition_ItemRequirement::IsSatisfied(AActor* TargetActor)
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

	return InventoryComponent->CanUseItem(Item.ItemID, Item.Count);
}
