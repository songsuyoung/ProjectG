#pragma once

#include "CoreMinimal.h"
#include "GInteractableActor.h"
#include "GKey.generated.h"

UCLASS()
class PROJECTG_API AGKey : public AGInteractableActor
{
	GENERATED_BODY()
	
public:	
	AGKey();
	
protected:
	
	virtual void InternalInteract(AActor* TargetActor) override;
};
