#pragma once

#include "CoreMinimal.h"
#include "System/GUIManagerBase.h"
#include "Data/GGameplayTags.h"
#include "UI/Base/GCommonActivatableWidget.h"
#include "GHUDIngame.generated.h"

UCLASS()
class PROJECTGCLIENT_API UGHUDIngame : public UGCommonActivatableWidget
{
	GENERATED_BODY()

public:
	static UGHUDIngame* OpenWindow(UGUIManagerBase* UIManager)
	{
		return Cast<UGHUDIngame>(UIManager->OpenWindow(GGameplayTags::UITag_Window_HUD, GGameplayTags::UITag_Layout_HUD));
	}
};
