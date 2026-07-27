#include "GUIWindowLoadMethod.h"

UClass* UGUIWindowLoadMethod::LoadAssetClass_Implementation(UClass* Class)
{
	if (false == IsValid(Class))
	{
		return nullptr;
	}
	
	TSoftClassPtr<UGCommonActivatableWidget>* SearchResult = WindowMap.Find(Class->GetFName());

	if (nullptr == SearchResult)
	{
		return nullptr;
	}

	// 동기적으로 로딩
	return SearchResult->LoadSynchronous();
}
