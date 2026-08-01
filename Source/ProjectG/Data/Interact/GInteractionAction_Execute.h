// Source/ProjectG/Data/Interact/GInteractionAction_Execute.h
#pragma once

#include "CoreMinimal.h"
#include "Data/Interact/GInteractionAction.h"
#include "GInteractionAction_Execute.generated.h"

UCLASS()
class PROJECTG_API UGInteractionAction_Execute : public UGInteractionAction
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
};
