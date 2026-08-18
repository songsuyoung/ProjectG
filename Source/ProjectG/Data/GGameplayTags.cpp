// Copyright Epic Games, Inc. All Rights Reserved.

#include "GGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
namespace GGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UITag_Layout_HUD, "UI.Layout.HUD", "UI");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UITag_Layout_Popup, "UI.Layout.Popup", "UI");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UITag_Window, "UI.Window", "UI window parent");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UITag_Window_HUD, "UI.Window.HUD", "UI window");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UITag_Window_Inventory, "UI.Window.Inventory", "UI window");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UITag_Window_Dialogue, "UI.Window.Dialogue", "UI window");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Open_DataTable, "Event.Open.DataTable", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Interact,          "Event.Interact",          "Interact events parent");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Interact_Detect,   "Event.Interact.Detect",   "Interactable detected");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Interact_Undetect, "Event.Interact.Undetect", "Interactable lost");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Interact_Started,  "Event.Interact.Started",  "Interaction started");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Interact_Ended,    "Event.Interact.Ended",    "Interaction ended");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Item,         "Event.Item",         "Item events parent");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Item_Acquired,"Event.Item.Acquired","Item added to inventory");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Item_Removed, "Event.Item.Removed", "Item removed from inventory");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Dialogue_Toggle, "Event.Dialogue.Toggle", "Toggle dialogue widget");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Dialogue_Node,   "Event.Dialogue.Node",   "Dialogue node updated");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Dialogue_SelectChoice,"Event.Dialogue.SelectChoice", "Player selected a dialogue choice");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Quest, "Event.Quest", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventTag_Quest_Accept, "Event.Quest.Accept", "Player accepted a quest via dialogue choice");

}

