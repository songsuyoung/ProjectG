#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UI/Base/GPrimaryWidget.h"
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
	
public:
	
	virtual void Init(UGPrimaryWidget* Root);
protected:
	
	UGCommonActivatableWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EGWidgetInputMode InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode;

public:
	bool IsPoolable() const { return bPoolable; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	bool bPoolable = false;
	
protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<UGPrimaryWidget> PrimaryWidget;
	
	friend class UGPrimaryWidget;
};
