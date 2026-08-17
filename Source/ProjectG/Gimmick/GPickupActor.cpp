#include "Gimmick/GPickupActor.h"

#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"
#include "Data/GInteractionPromptRow.h"
#include "System/GDataManager.h"

AGPickupActor::AGPickupActor()
	: Super()
{
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
