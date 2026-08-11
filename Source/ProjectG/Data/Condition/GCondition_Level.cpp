#include "GCondition_Level.h"

#include "Character/GCharacter.h"
#include "Data/GConditionRow.h"

bool UGCondition_Level::IsSatisfied(AActor* TargetActor, int32 Level)
{
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);

	if (IsValid(Character))
	{
		return Character->GetLevel() >= Level;
	}

	return false;
}

bool UGCondition_Level::IsSatisfied(AActor* TargetActor, FGConditionRow* Condition)
{
	return IsSatisfied(TargetActor, Condition->IntParam);
}
