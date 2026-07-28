// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace GGameplayTags
{
	PROJECTG_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);
	
	PROJECTG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(UITag_Layout_HUD);
};
