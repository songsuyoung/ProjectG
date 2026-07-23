#include "Player/GPlayerController.h"

#include "Component/GHeroComponent.h"

AGPlayerController::AGPlayerController(const FObjectInitializer& ObjectInitializer)
{
	HeroComponent = CreateDefaultSubobject<UGHeroComponent>("HeroComponent");
}

void AGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (IsValid(HeroComponent))
	{
		HeroComponent->SetupInputComponent();
	}
}