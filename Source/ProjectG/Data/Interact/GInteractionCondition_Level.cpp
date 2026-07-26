#include "Data/Interact/GInteractionCondition_Level.h"

#include "Character/GCharacter.h"

bool UGInteractionCondition_Level::IsSatisfied(AActor* TargetActor)
{
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);
	
	if (IsValid(Character))
	{
		return Character->GetLevel() >= Level;
	}
	
	return false;
}
