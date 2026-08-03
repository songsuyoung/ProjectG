#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GInteractionAction.h"
#include "GInteractionAction_OpenUI.generated.h"

class AGInteractableActor;

UCLASS(Blueprintable)
class PROJECTG_API UGInteractionAction_OpenUI : public UGInteractionAction
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Settings")
	FGameplayTag EventTag_UI;
};
