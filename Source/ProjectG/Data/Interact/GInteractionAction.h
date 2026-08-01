// Source/ProjectG/Data/Interact/GInteractionAction.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GInteractionAction.generated.h"

UCLASS(Abstract)
class PROJECTG_API UGInteractionAction : public UObject
{
    GENERATED_BODY()

public:
    FSimpleDelegate OnCompleted;

    virtual void Execute(AActor* OwnerActor, AActor* TargetActor) PURE_VIRTUAL(UGInteractionAction::Execute, );
    virtual void Tick(float DeltaTime) {}

protected:
    void Finish();
};
