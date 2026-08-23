#include "Gimmick/GPickupActor.h"

#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"
#include "Data/GInteractionPromptRow.h"
#include "System/GDataManager.h"

AGPickupActor::AGPickupActor()
	: Super()
{
}

bool AGPickupActor::CanInteract(AActor* TargetActor)
{
	bool bResult = Super::CanInteract(TargetActor);
	
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	FGInteractionPromptRow* InteractionPromptRow = DataManager->GetDataTableRow<FGInteractionPromptRow>(EGDataTableType::InteractionPrompt, ID);
	
	if (nullptr == InteractionPromptRow)
	{
		return false;
	}
	
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);

	if (IsValid(Character))
	{
		UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();

		if (IsValid(InventoryComponent))
		{
			return bResult && InventoryComponent->CanAcquire(InteractionPromptRow->ItemID);
		}
	}
	
	return bResult;
}

void AGPickupActor::InternalInteract(AActor* TargetActor)
{
	Super::InternalInteract(TargetActor);

	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	FGInteractionPromptRow* InteractionPromptRow = DataManager->GetDataTableRow<FGInteractionPromptRow>(EGDataTableType::InteractionPrompt, ID);
	
	if (nullptr == InteractionPromptRow)
	{
		return;
	}
	
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);

	if (IsValid(Character))
	{
		UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();

		if (IsValid(InventoryComponent))
		{
			InventoryComponent->Acquire(InteractionPromptRow->ItemID);
		}
	}

	SetLifeSpan(0.3f);
}
