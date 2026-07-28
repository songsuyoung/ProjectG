#include "GUIManagerBase.h"

#include "GGameInstance.h"

UGUIManagerBase* UGUIManagerBase::Get(UObject* Context)
{
	UWorld* World = Context->GetWorld();
	
	check(World);
	
	
	UGGameInstance* GameInstance = Cast<UGGameInstance>(World->GetGameInstance());
	
	check(GameInstance);
	
	return GameInstance->GetUIManager();
}
