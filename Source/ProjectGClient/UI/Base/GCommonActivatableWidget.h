#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GCommonActivatableWidget.generated.h"

struct FUIInputConfig;

UENUM(BlueprintType)
enum class EGWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

UCLASS()
class PROJECTGCLIENT_API UGCommonActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	
	UGCommonActivatableWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EGWidgetInputMode InputConfig;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode;
};
