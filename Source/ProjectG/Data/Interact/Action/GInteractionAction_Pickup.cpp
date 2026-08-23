#include "GInteractionAction_Pickup.h"

#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"
#include "Gimmick/GInteractableActor.h"
#include "Gimmick/GPickupActor.h"

void UGInteractionAction_Pickup::Execute(AActor* OwnerActor, AActor* TargetActor)
{
	AGPickupActor* PickupActor = Cast<AGPickupActor>(OwnerActor);
	
	if (false == IsValid(PickupActor))
	{
		Finish();
		return;
	}
	
	FName ItemID = PickupActor->GetID();
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);

	if (IsValid(Character))
	{
		UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();

		if (IsValid(InventoryComponent))
		{
			if (InventoryComponent->CanAcquire(ItemID))
			{
				InventoryComponent->Acquire(ItemID);
				
				PickupActor->SetLifeSpan(LifeSpan);
			}
		}
	}
	
	Finish();
}
