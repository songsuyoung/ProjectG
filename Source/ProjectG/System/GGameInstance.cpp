#include "GGameInstance.h"


#include "GDataManager.h"
#include "GConditionManager.h"
#include "GDialogueManager.h"
#include "GEventManager.h"
#include "GUIManagerBase.h"

void UGGameInstance::Init()
{
	Super::Init();
	
	DialogueManager = NewObject<UGDialogueManager>(this, UGDialogueManager::StaticClass());
	EventManager = NewObject<UGEventManager>(this, UGEventManager::StaticClass());
	DataManager = NewObject<UGDataManager>(this, DataManagerClass);
	UIManager = NewObject<UGUIManagerBase>(this, UIManagerClass);
	ConditionManager = NewObject<UGConditionManager>(this, ConditionManagerClass);
	// UObject인 Subsystem의 Initialize 를 합칠 Interface 구축 또는 UObject 구축 
	if (IsValid(DataManager))
	{
		DataManager->Initialize();
	}
}
