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

	for (UGCondition* Condition : Conditions)
	{
		if (UGInteractionCondition* InteractCondition = Cast<UGInteractionCondition>(Condition))
		{
			InteractCondition->Init(this);
		}
	}
}

void AGNPCCharacter::InternalInteract(AActor* TargetActor)
{
}
