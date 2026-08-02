#include "GHUDIngame.h"

#include "Data/GGameMacro.h"
#include "Data/GMessage.h"
#include "System/GEventManager.h"
#include "UI/GInventoryWidget.h"

void UGHUDIngame::NativeConstruct()
{
	Super::NativeConstruct();
	GEVENT_MESSAGE_ADD(this, this);
}

void UGHUDIngame::NativeDestruct()
{
	Super::NativeDestruct();
	GEVENT_MESSAGE_REMOVE(this, this);
}

void UGHUDIngame::OnMessage(EGMessageType Type, FGMessage* Message)
{
	bCanShowInventory = !bCanShowInventory;
	
	switch (Type)
	{
	case EGMessageType::InventoryToggle:
		{
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
		break;
	}
}
