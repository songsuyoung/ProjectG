
#pragma once

#include "CoreMinimal.h"
#include "Data/Interact/Action/GInteractionAction.h"
#include "GInteractionAction_UseItem.generated.h"

UCLASS()
class PROJECTG_API UGInteractionAction_UseItem : public UGInteractionAction
{
    GENERATED_BODY()

public:
    virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
};
