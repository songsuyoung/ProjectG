#include "Data/Interact/GInteractionCondition_ItemRequirement.h"

#include "Character/GCharacter.h"

bool UGInteractionCondition_ItemRequirement::IsSatisfied(AActor* TargetActor)
{
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);
	
	if (IsValid(Character))
	{
		return Character->ContainInventory(Item);
	}
	
	return false;
}
