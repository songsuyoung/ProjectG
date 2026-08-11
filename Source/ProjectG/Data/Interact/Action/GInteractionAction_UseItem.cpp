#include "GInteractionAction_UseItem.h"

#include "Character/GCharacter.h"
#include "Component/GInteractionActionComponent.h"
#include "Component/GInventoryComponent.h"
#include "Data/GConditionRow.h"
#include "Gimmick/GInteractableActor.h"
#include "System/GDataManager.h"

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
        
        UGDataManager* DataManager = UGDataManager::Get(this);
        check(DataManager);
        
        for (const FName& ConditionID : IAC->GetConditions())
        {
            FGConditionRow* ConditionRow = DataManager->GetDataTableRow<FGConditionRow>(EGDataTableType::Condition, ConditionID);
            
            if (nullptr != ConditionRow)
            {
                InventoryComponent->UseItem(ConditionRow->IDParam, ConditionRow->IntParam);
            }
        }
    }

    Finish();
}
