#include "System/GConditionManager.h"

#include "GDataManager.h"
#include "GGameInstance.h"
#include "Data/GConditionRow.h"
#include "Data/Condition/GCondition.h"

UGConditionManager* UGConditionManager::Get(const UObject* Context)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}

	const UWorld* World = Context->GetWorld();
	if (!IsValid(World))
	{
		return nullptr;
	}

	const UGGameInstance* GameInstance = Cast<UGGameInstance>(World->GetGameInstance());
	return IsValid(GameInstance) ? GameInstance->GetConditionManager() : nullptr;
}

bool UGConditionManager::IsSatisfied(const FName& ConditionID, AActor* RelatedActor)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	FGConditionRow* ConditionRow = DataManager->GetDataTableRow<FGConditionRow>(
		EGDataTableType::Condition,
		ConditionID);

	if (nullptr != ConditionRow)
	{
		TSubclassOf<UGCondition>* Condition = Conditions.Find(ConditionRow->ConditionType);

		if (nullptr != Condition)
		{
			UGCondition* ConditionCDO = Condition->GetDefaultObject();

			if (IsValid(ConditionCDO))
			{
				return ConditionCDO->IsSatisfied(RelatedActor, ConditionRow);
			}
		}
	}

	return false;
}
