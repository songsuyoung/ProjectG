#include "GInteractionCondition_CanAcquire.h"

#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"
#include "Data/GConditionRow.h"

bool UGInteractionCondition_CanAcquire::IsSatisfied(AActor* TargetActor, FGConditionRow* Condition)
{
	return IsSatisfied(TargetActor, Condition->IDParam);
}

bool UGInteractionCondition_CanAcquire::IsSatisfied(AActor* TargetActor, FName ItemID)
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

	return InventoryComponent->CanAcquire(ItemID);
}
