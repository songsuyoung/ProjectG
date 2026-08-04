#include "GInteractionAction_OpenUI.h"

#include "Data/GGameMacro.h"
#include "System/GEventManager.h"

void UGInteractionAction_OpenUI::Execute(AActor* OwnerActor, AActor* TargetActor)
{
	GEVENT_BROADCAST_EMPTY(OwnerActor, EventTag_UI);
	Finish();
}
