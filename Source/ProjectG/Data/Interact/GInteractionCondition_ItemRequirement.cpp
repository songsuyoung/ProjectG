#include "Data/Interact/GInteractionCondition_ItemRequirement.h"

#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"

bool UGInteractionCondition_ItemRequirement::IsSatisfied(AActor* TargetActor)
{
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);
	if (!IsValid(Character)) return false;

	UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
	if (!IsValid(InventoryComponent)) return false;

	return InventoryComponent->CanUseItem(Item.ItemID, Item.Count);
}

void UGInteractionCondition_ItemRequirement::Apply(AActor* TargetActor)
{
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);
	if (!IsValid(Character)) return;

	UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
	if (!IsValid(InventoryComponent)) return;

	InventoryComponent->UseItem(Item.ItemID, Item.Count);
}
