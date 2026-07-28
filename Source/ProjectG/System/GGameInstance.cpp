#include "GGameInstance.h"

#include "GDataManager.h"
#include "GEventManager.h"
#include "GUIManagerBase.h"

void UGGameInstance::Init()
{
	Super::Init();

	EventManager = NewObject<UGEventManager>(this, UGEventManager::StaticClass());
	DataManager = NewObject<UGDataManager>(this, DataManagerClass);
	UIManager = NewObject<UGUIManagerBase>(this, UIManagerClass);
	
	// UObject인 Subsystem의 Initialize 를 합칠 Interface 구축 또는 UObject 구축 
	if (IsValid(DataManager))
	{
		DataManager->Initialize();
	}
}