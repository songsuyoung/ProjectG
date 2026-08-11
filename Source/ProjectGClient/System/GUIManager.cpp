#include "GUIManager.h"

#include "UI/GHUDBase.h"

UGUIManager* UGUIManager::Get(UObject* Context)
{
	return Cast<UGUIManager>(UGUIManagerBase::Get(Context));
}

void UGUIManager::Initialize(AGHUDBase* HUDBase, TSubclassOf<UGPrimaryWidget> RootWidgetClass)
{
	RootWidget = CreateWidget<UGPrimaryWidget>(HUDBase->GetOwningPlayerController(), RootWidgetClass);

	if (IsValid(RootWidget))
	{
		RootWidget->AddToViewport();
	}
	
	HUD = HUDBase;
}

UGCommonActivatableWidget* UGUIManager::OpenWindowInternalImpl(UClass* WidgetClass, FGameplayTag LayerTag)
{
	UGCommonActivatableWidget* CommonActivatableWidget = nullptr;

	if (IsValid(RootWidget))
	{
		CommonActivatableWidget = RootWidget->GetOrCreateInstance(WidgetClass, LayerTag);
	}

	return CommonActivatableWidget;
}

void UGUIManager::CloseWindowInternalImpl(UGCommonActivatableWidget* Widget)
{
	if (IsValid(RootWidget))
	{
		RootWidget->ReturnToPool(Widget);
	}
}
