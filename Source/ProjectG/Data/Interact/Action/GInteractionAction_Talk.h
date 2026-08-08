#pragma once

#include "CoreMinimal.h"
#include "GInteractionAction.h"
#include "GInteractionAction_Talk.generated.h"

UCLASS(Blueprintable)
class PROJECTG_API UGInteractionAction_Talk : public UGInteractionAction
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
};
