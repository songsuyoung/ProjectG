#include "GUIWindowLoadMethod.h"

#include "Data/GGameplayTags.h"

UClass* UGUIWindowLoadMethod::LoadAssetClass_Implementation(FGameplayTag WindowTag)
{
	if (false == WindowTag.IsValid())
	{
		return nullptr;
	}
	
	TSoftClassPtr<UGCommonActivatableWidget>* SearchResult = WindowMap.Find(WindowTag);

	if (nullptr == SearchResult)
	{
		return nullptr;
	}

	// 동기적으로 로딩
	return SearchResult->LoadSynchronous();
}
