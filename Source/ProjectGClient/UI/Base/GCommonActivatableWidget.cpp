#include "GCommonActivatableWidget.h"

#include "Engine/EngineBaseTypes.h"

UGCommonActivatableWidget::UGCommonActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InputConfig(EGWidgetInputMode::Default)
	, GameMouseCaptureMode(EMouseCaptureMode::CapturePermanently)
{
}

TOptional<FUIInputConfig> UGCommonActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EGWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EGWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EGWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case EGWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
