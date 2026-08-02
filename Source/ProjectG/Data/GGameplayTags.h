// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace GGameplayTags
{
	PROJECTG_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(UITag_Layout_HUD);

	// Event tags - Interact
	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Interact);
	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Interact_Detect);
	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Interact_Undetect);
	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Interact_Started);
	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Interact_Ended);

	// Event tags - Item
	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Item);
	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Item_Acquired);
	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_Item_Removed);

	// Event tags - UI
	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventTag_UI_InventoryToggle);
};
