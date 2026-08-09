#include "GInteractionCondition_CanAcquire.h"

#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"

bool UGInteractionCondition_CanAcquire::IsSatisfied(AActor* TargetActor)
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

	return InventoryComponent->CanAcquire(Interactable->GetInteractionID());
}
