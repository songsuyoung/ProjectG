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
	
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);

	if (IsValid(Character))
	{
		UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();

		if (IsValid(InventoryComponent))
		{
			return bResult && InventoryComponent->CanAcquire(ItemID);
		}
	}
	
	return bResult;
}

void AGPickupActor::InternalInteract(AActor* TargetActor)
{
	Super::InternalInteract(TargetActor);
	
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);

	if (IsValid(Character))
	{
		UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();

		if (IsValid(InventoryComponent))
		{
			InventoryComponent->Acquire(ItemID);
		}
	}

	SetLifeSpan(0.3f);
}
