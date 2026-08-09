#include "Character/GNPCCharacter.h"

#include "Component/GInteractionActionComponent.h"
#include "Data/Condition/Interact/GInteractionCondition.h"

AGNPCCharacter::AGNPCCharacter()
	: Super()
{
	InteractionActionComponent = CreateDefaultSubobject<UGInteractionActionComponent>(TEXT("InteractionActionComponent"));
}

void AGNPCCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AGNPCCharacter::InternalInteract(AActor* TargetActor)
{
}
