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

	void SetID(FName NewID) { ItemID = NewID; }
	FName GetID() { return ItemID; }
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Item")
	FName ItemID;
};
