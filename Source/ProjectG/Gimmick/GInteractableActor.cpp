#include "Gimmick/GInteractableActor.h"

#include "Character/GCharacter.h"
#include "Component/GInteractionActionComponent.h"
#include "Data/GGameEnums.h"
#include "Data/Condition/Interact/GInteractionCondition.h"
#include "GameFramework/PlayerController.h"

AGInteractableActor::AGInteractableActor()
	: Super()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComponent"));
	RootComponent = MeshComponent;

	InteractionActionComponent = CreateDefaultSubobject<UGInteractionActionComponent>(TEXT("InteractionActionComponent"));
	InteractPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractPoint"));
	InteractPoint->SetupAttachment(GetRootComponent());
}

FTransform AGInteractableActor::GetInteractPointTransform() const
{
	if (false == IsValid(InteractPoint))
	{
		return GetActorTransform();
	}

	return InteractPoint->GetComponentTransform();
}
