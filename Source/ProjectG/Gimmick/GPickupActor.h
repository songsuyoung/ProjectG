#pragma once

#include "CoreMinimal.h"
#include "GInteractableActor.h"
#include "GPickupActor.generated.h"

UCLASS()
class PROJECTG_API AGPickupActor : public AGInteractableActor
{
	GENERATED_BODY()

public:
	AGPickupActor();

protected:

	virtual void InternalInteract(AActor* TargetActor) override;
};
