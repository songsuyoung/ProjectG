#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "GCommonActivatableWidgetStack.generated.h"

UCLASS()
class PROJECTGCLIENT_API UGCommonActivatableWidgetStack : public UCommonActivatableWidgetStack
{
	GENERATED_BODY()
public:
	
	const FGameplayTag& GetLayerType() { return LayerTag; }
protected:
	
	UPROPERTY(EditAnywhere, Category = "Layer|Category")
	FGameplayTag LayerTag;
};
