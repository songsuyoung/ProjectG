// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
namespace GGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UITag_Layout_HUD, "UI.Layout.HUD", "UI");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Interact,          "Event.Interact",          "Interact events parent");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Interact_Detect,   "Event.Interact.Detect",   "Interactable detected");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Interact_Undetect, "Event.Interact.Undetect", "Interactable lost");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Interact_Started,  "Event.Interact.Started",  "Interaction started");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Interact_Ended,    "Event.Interact.Ended",    "Interaction ended");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Item,         "Event.Item",         "Item events parent");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Item_Acquired,"Event.Item.Acquired","Item added to inventory");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Item_Removed, "Event.Item.Removed", "Item removed from inventory");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_UI_InventoryToggle, "Event.UI.InventoryToggle", "Toggle inventory UI");
}

