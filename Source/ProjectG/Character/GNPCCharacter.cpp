#include "Character/GNPCCharacter.h"

#include "Component/GDialogueComponent.h"
#include "Component/GInteractionActionComponent.h"
#include "Data/Interact/GInteractionCondition.h"

AGNPCCharacter::AGNPCCharacter()
	: Super()
{
	InteractionActionComponent = CreateDefaultSubobject<UGInteractionActionComponent>(TEXT("InteractionActionComponent"));
	DialogueComponent = CreateDefaultSubobject<UGDialogueComponent>(TEXT("DialogueComponent"));
}

void AGNPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (UGInteractionCondition* Condition : Conditions)
	{
		if (IsValid(Condition))
		{
			Condition->Init(this);
		}
	}
}

void AGNPCCharacter::InternalInteract(AActor* TargetActor)
{
}
