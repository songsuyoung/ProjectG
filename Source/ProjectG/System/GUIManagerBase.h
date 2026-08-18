#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GUIManagerBase.generated.h"

class UGCommonActivatableWidget;
class AGHUDBase;
class UGPrimaryWidget;

UCLASS(Blueprintable)
class PROJECTG_API UGUIManagerBase : public UObject
{
	GENERATED_BODY()
	
public:
	static UGUIManagerBase* Get(UObject* Context);
	
	virtual void Initialize(AGHUDBase* HUDBase, TSubclassOf<UGPrimaryWidget> RootWidgetClass) { };
	UGCommonActivatableWidget* OpenWindow(FGameplayTag WindowTag, FGameplayTag LayerTag) { return OpenWindowInternalImpl(WindowTag, LayerTag); }
	void CloseWindow(UGCommonActivatableWidget* Widget) { CloseWindowInternalImpl(Widget); }
protected:
	virtual UGCommonActivatableWidget* OpenWindowInternalImpl(FGameplayTag WindowTag, FGameplayTag LayerTag) { return nullptr; };
	virtual void CloseWindowInternalImpl(FGameplayTag WindowTag) {};
	virtual void CloseWindowInternalImpl(UGCommonActivatableWidget* Widget) {};
};
