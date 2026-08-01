#include "GInteractionAction_Execute.h"
#include "Gimmick/GInteractableActor.h"

void UGInteractionAction_Execute::Execute(AActor* OwnerActor, AActor* TargetActor)
{
	AGInteractableActor* Interactable = Cast<AGInteractableActor>(OwnerActor);
	if (false == IsValid(Interactable))
	{
		Finish();
		return;
	}

	Interactable->InternalInteract(TargetActor);
	Finish();
}
