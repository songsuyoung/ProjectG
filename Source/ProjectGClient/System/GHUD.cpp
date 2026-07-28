#include "GHUD.h"

#include "GUIManager.h"
#include "UI/HUD/GHUDIngame.h"

void AGHUD::Init()
{
	Super::Init();
	
	UGUIManager* UIManager = UGUIManager::Get(this);
	
	ensure(UIManager);
	
	UIManager->Initialize(this, RootWidgetClass);
	
	UGHUDIngame::OpenWindow(UIManager, UGHUDIngame::StaticClass());
}
