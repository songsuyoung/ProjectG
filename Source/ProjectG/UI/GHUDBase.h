#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GHUDBase.generated.h"

UCLASS()
class PROJECTG_API AGHUDBase : public AHUD
{
	GENERATED_BODY()
	
public:
	
	virtual void Init() { }
	
};
