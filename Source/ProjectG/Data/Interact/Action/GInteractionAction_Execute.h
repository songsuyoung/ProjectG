#pragma once

#include "CoreMinimal.h"
#include "GInteractionAction.h"
#include "GInteractionAction_Execute.generated.h"

UCLASS()
class PROJECTG_API UGInteractionAction_Execute : public UGInteractionAction
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
};
