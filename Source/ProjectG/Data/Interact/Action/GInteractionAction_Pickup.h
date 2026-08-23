#pragma once

#include "CoreMinimal.h"
#include "GInteractionAction.h"
#include "GInteractionAction_Pickup.generated.h"

UCLASS()
class PROJECTG_API UGInteractionAction_Pickup : public UGInteractionAction
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Item")
	float LifeSpan;
};
