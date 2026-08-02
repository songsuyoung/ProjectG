#include "Gimmick/GInteractableActor.h"

#include "Character/GCharacter.h"
#include "Component/GInteractionActionComponent.h"
#include "Data/GGameEnums.h"
#include "Data/Interact/GInteractionCondition.h"
#include "GameFramework/PlayerController.h"

AGInteractableActor::AGInteractableActor()
	: Super()
	, InteractionState(EGInteractionState::Available)
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComponent"));
	RootComponent = MeshComponent;
	
	InteractionActionComponent = CreateDefaultSubobject<UGInteractionActionComponent>(TEXT("ActionPipeline"));
	InteractPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractPoint"));
	InteractPoint->SetupAttachment(GetRootComponent());
}

EGInteractionState AGInteractableActor::GetInteractionState(AActor* TargetActor)
{
	if (InteractionState == EGInteractionState::Unavailable)
	{
		return InteractionState;
	}

	for (int32 Index = 0; Index < Conditions.Num(); Index++)
	{
		if (false == Conditions[Index]->IsSatisfied(TargetActor))
		{
			InteractionState = EGInteractionState::Pending;
			return InteractionState;
		}
	}

	InteractionState = EGInteractionState::Available;
	return InteractionState;
}

void AGInteractableActor::OnInteractStarted(AActor* TargetActor)
{
	UWorld* World = GetWorld();
	check(World);

	StartTimestamp = World->GetTimeSeconds();
}

bool AGInteractableActor::CanInteract(AActor* TargetActor)
{
	UWorld* World = GetWorld();
	check(World);

	float EndTimestamp = World->GetTimeSeconds();
	float Duration = (EndTimestamp - StartTimestamp);

	return Duration >= HoldDuration && GetInteractionState(TargetActor) == EGInteractionState::Available;
}

void AGInteractableActor::Interact(AActor* TargetActor)
{
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);
	if (false == IsValid(Character))
	{
		return;
	}

	if (false == IsValid(InteractionActionComponent))
	{
		return;
	}

	InteractingCharacter = Character;

	APlayerController* PC = Cast<APlayerController>(Character->GetController());
	if (nullptr != PC)
	{
		PC->SetIgnoreMoveInput(true);
	}

	FSimpleDelegate OnCompleted = FSimpleDelegate::CreateUObject(this, &ThisClass::OnInteractionCompleted);
	InteractionActionComponent->Run(this, TargetActor, OnCompleted);
	
}

void AGInteractableActor::InternalInteract(AActor* TargetActor)
{
	for (UGInteractionCondition* Condition : Conditions)
	{
		Condition->Apply(TargetActor);
	}
}

void AGInteractableActor::OnInteractionCompleted()
{
	InteractionState = EGInteractionState::Unavailable;

	if (InteractingCharacter.IsValid())
	{
		APlayerController* PC = Cast<APlayerController>(InteractingCharacter->GetController());
		if (nullptr != PC)
		{
			PC->ResetIgnoreMoveInput();
		}
		InteractingCharacter = nullptr;
	}
}

FTransform AGInteractableActor::GetInteractPointTransform() const
{
	if (false == IsValid(InteractPoint))
	{
		return GetActorTransform();
	}

	return InteractPoint->GetComponentTransform();
}

UAnimMontage* AGInteractableActor::GetInteractMontage() const
{
	return InteractMontage.Get();
}

void AGInteractableActor::BeginPlay()
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

