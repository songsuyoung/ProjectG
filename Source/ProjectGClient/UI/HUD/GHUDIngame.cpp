#include "GHUDIngame.h"

#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "System/GEventManager.h"
#include "UI/GInventoryWidget.h"

void UGHUDIngame::NativeConstruct()
{
	Super::NativeConstruct();
	GEVENT_ADD(this, GGameplayTags::EventTag_UI_InventoryToggle, this);
}

void UGHUDIngame::NativeDestruct()
{
	Super::NativeDestruct();
	GEVENT_REMOVE(this, GGameplayTags::EventTag_UI_InventoryToggle, this);
}

void UGHUDIngame::OnMessage(FGameplayTag Tag, FGMessage* Message)
{
	bCanShowInventory = !bCanShowInventory;

	if (IsValid(InventoryWidget))
	{
		if (bCanShowInventory)
		{
			InventoryWidget->ActivateWidget();
		}
		else
		{
			InventoryWidget->DeactivateWidget();
		}
	}
}
