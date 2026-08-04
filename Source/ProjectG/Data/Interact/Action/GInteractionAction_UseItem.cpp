#include "GInteractionAction_UseItem.h"

#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"
#include "Data/Interact/GInteractionCondition_ItemRequirement.h"
#include "Gimmick/GInteractableActor.h"

void UGInteractionAction_UseItem::Execute(AActor* OwnerActor, AActor* TargetActor)
{
    AGCharacter* Character = Cast<AGCharacter>(TargetActor);
    if (false == IsValid(Character))
    {
        Finish();
        return;
    }

    UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
    if (false == IsValid(InventoryComponent))
    {
        Finish();
        return;
    }

    AGInteractableActor* Interactable = Cast<AGInteractableActor>(OwnerActor);
    if (IsValid(Interactable))
    {
        for (UGInteractionCondition* Condition : Interactable->GetConditions())
        {
            UGInteractionCondition_ItemRequirement* ItemReq = Cast<UGInteractionCondition_ItemRequirement>(Condition);
            if (IsValid(ItemReq))
            {
                InventoryComponent->UseItem(ItemReq->GetItemID(), ItemReq->GetCount());
            }
        }
    }

    Finish();
}
