
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GPlayerController.generated.h"

class UGHeroComponent;

UCLASS()
class PROJECTG_API AGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AGPlayerController(const FObjectInitializer& ObjectInitializer);
	UGHeroComponent* GetHeroComponent() { return HeroComponent; }
	
protected:
	virtual void SetupInputComponent() override;
protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UGHeroComponent> HeroComponent;
};
