#pragma once

#include "CoreMinimal.h"
#include "UI/GHUDBase.h"
#include "GHUD.generated.h"

class UGPrimaryWidget;

UCLASS()
class PROJECTGCLIENT_API AGHUD : public AGHUDBase
{
	GENERATED_BODY()
	
public:
	
	virtual void Init() override;
	
		
protected:
	UPROPERTY(EditAnywhere, Category = "RootWidget")
	TSubclassOf<UGPrimaryWidget> RootWidgetClass;
};
