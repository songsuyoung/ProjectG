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
	static UGHUDIngame* OpenWindow(UGUIManagerBase* UIManager, UClass* Class)
	{
		return Cast<UGHUDIngame>(UIManager->OpenWindow(Class, GGameplayTags::UITag_Layout_HUD));
	}
};
