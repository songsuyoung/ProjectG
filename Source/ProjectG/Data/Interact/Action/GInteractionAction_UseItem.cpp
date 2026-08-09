#include "GInteractionAction_UseItem.h"

#include "Character/GCharacter.h"
#include "Component/GInteractionActionComponent.h"
#include "Component/GInventoryComponent.h"
#include "Data/Condition/Interact/GInteractionCondition.h"
#include "Data/Condition/GCondition_ItemRequirement.h"
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

    IGInteractable* Interactable = Cast<IGInteractable>(OwnerActor);
    if (nullptr != Interactable)
    {
        UGInteractionActionComponent* IAC = Interactable->GetInteractionActionComponent();
        
        if (false == IsValid(IAC))
        {
            return;
        }
        
        for (const TObjectPtr<UGInteractionCondition>& Condition : IAC->GetConditions())
        {
            const UGCondition_ItemRequirement* const ItemReq = Cast<UGCondition_ItemRequirement>(Condition);
            if (IsValid(ItemReq))
            {
                InventoryComponent->UseItem(ItemReq->GetItemID(), ItemReq->GetCount());
            }
        }
    }

    Finish();
}
