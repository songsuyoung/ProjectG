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
	UGCommonActivatableWidget* OpenWindow(UClass* WidgeClass, FGameplayTag LayerTag) { return OpenWindowInternalImpl(WidgeClass, LayerTag); }
	void CloseWindow(UGCommonActivatableWidget* Widget) { CloseWindowInternalImpl(Widget); }
protected:
	virtual UGCommonActivatableWidget* OpenWindowInternalImpl(UClass* WidgetClass, FGameplayTag LayerTag) { return nullptr; };
	virtual void CloseWindowInternalImpl(UGCommonActivatableWidget* Widget) {};
};
