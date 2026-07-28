#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "System/GUIManagerBase.h"
#include "UI/Base/GPrimaryWidget.h"
#include "GUIManager.generated.h"

class UGCommonActivatableWidget;
class AGHUDBase;
class UGPrimaryWidget;

UCLASS()
class PROJECTGCLIENT_API UGUIManager : public UGUIManagerBase
{
	GENERATED_BODY()
	
public:
	static UGUIManager* Get(UObject* Context);
	
	virtual void Initialize(AGHUDBase* HUDBase, TSubclassOf<UGPrimaryWidget> RootWidgetClass) override;

protected:
	virtual UGCommonActivatableWidget* OpenWindowInternalImpl(UClass* WidgetClass, FGameplayTag LayerTag) override;;
	
protected:
	
	UPROPERTY(Transient)
	TObjectPtr<UGPrimaryWidget> RootWidget;
	
	UPROPERTY(Transient)
	TObjectPtr<AGHUDBase> HUD;

};
