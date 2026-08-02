#pragma once

#include "CoreMinimal.h"
#include "System/GUIManagerBase.h"
#include "Data/GGameplayTags.h"
#include "Interface/GMessageReceiver.h"
#include "UI/Base/GCommonActivatableWidget.h"
#include "GHUDIngame.generated.h"

class UGInventoryWidget;

UCLASS()
class PROJECTGCLIENT_API UGHUDIngame : public UGCommonActivatableWidget, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	static UGHUDIngame* OpenWindow(UGUIManagerBase* UIManager, UClass* Class)
	{
		return Cast<UGHUDIngame>(UIManager->OpenWindow(Class, GGameplayTags::UITag_Layout_HUD));
	}
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
protected:

	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;

protected:
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UGInventoryWidget> InventoryWidget;
	
	
protected:
	UPROPERTY(Transient)
	uint8 bCanShowInventory : 1; 
};
