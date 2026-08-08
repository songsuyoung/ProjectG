#include "GHUDIngame.h"

#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "System/GEventManager.h"
#include "UI/GCraftingWidget.h"
#include "UI/GDialogueWidget.h"
#include "UI/GInventoryWidget.h"

void UGHUDIngame::NativeConstruct()
{
	Super::NativeConstruct();
	GEVENT_ADD(this, GGameplayTags::EventTag_UI_InventoryToggle, this);
	GEVENT_ADD(this, GGameplayTags::EventTag_UI_CraftingToggle, this);
	GEVENT_ADD(this, GGameplayTags::EventTag_Dialogue_Toggle, this);
}

void UGHUDIngame::NativeDestruct()
{
	Super::NativeDestruct();
	GEVENT_REMOVE(this, GGameplayTags::EventTag_UI_InventoryToggle, this);
	GEVENT_REMOVE(this, GGameplayTags::EventTag_UI_CraftingToggle, this);
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Dialogue_Toggle, this);
}

void UGHUDIngame::OnMessage(FGameplayTag Tag, FGMessage* Message)
{
	if (Tag == GGameplayTags::EventTag_UI_InventoryToggle)
	{
		if (IsValid(InventoryWidget))
		{
			InventoryWidget->ActivateWidget();
		}
	}
	else if (Tag == GGameplayTags::EventTag_UI_CraftingToggle)
	{
		if (IsValid(CraftingWidget))
		{
			CraftingWidget->ActivateWidget();
		}
	}
	else if (Tag == GGameplayTags::EventTag_Dialogue_Toggle)
	{
		if (IsValid(DialogueWidget))
		{
			if (DialogueWidget->IsActivated())
			{
				DialogueWidget->DeactivateWidget();
			}
			else
			{
				DialogueWidget->ActivateWidget();
			}
		}
	}
}
