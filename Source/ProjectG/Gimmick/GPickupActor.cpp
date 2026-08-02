#include "Gimmick/GPickupActor.h"

#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"

AGPickupActor::AGPickupActor()
	: Super()
{
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
			InventoryComponent->Acquire(ID);
		}
	}

	SetLifeSpan(0.3f);
}
