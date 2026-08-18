#include "GInteractionAction_OpenUI.h"

#include "Data/GGameplayTags.h"
#include "System/GUIManagerBase.h"

void UGInteractionAction_OpenUI::Execute(AActor* OwnerActor, AActor* TargetActor)
{
	UGUIManagerBase* UIManager = UGUIManagerBase::Get(this);
	
	check(UIManager);
	
	UIManager->OpenWindow(EventTag_UI, GGameplayTags::UITag_Layout_Popup);

	Finish();
}
