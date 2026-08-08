#include "Component/GInteractionComponent.h"

// UE 5.
#include "Components/SphereComponent.h"
#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GMessage.h"
#include "Data/GGameEnums.h"
#include "GameFramework/Character.h"
#include "Interface/GInteractable.h"
#include "System/GEventManager.h"

UGInteractionComponent::UGInteractionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ViewDotThreshold(0.75f)
{
	InteractionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphereComponent"));
	InteractionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &UGInteractionComponent::OnSphereBeginOverlap);
	InteractionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &UGInteractionComponent::OnSphereEndOverlap);

	bWantsInitializeComponent = true;

	PrimaryComponentTick.bCanEverTick = true;
}

void UGInteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	CharacterRef = Cast<ACharacter>(GetOwner());

	if (CharacterRef.IsValid())
	{
		InteractionSphereComponent->AttachToComponent(CharacterRef->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void UGInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateFocusTarget();
}

void UGInteractionComponent::UpdateFocusTarget()
{
	// 무효화된 액터 또는 Unavailable 상태인 액터를 제거한다.
	for (int32 Index = InteractableCandidates.Num() - 1; Index >= 0; --Index)
	{
		const TWeakObjectPtr<AActor>& Actor = InteractableCandidates[Index].Actor;
		if (false == Actor.IsValid())
		{
			InteractableCandidates.RemoveAtSwap(Index);
			continue;
		}

		IGInteractable* Interactable = Cast<IGInteractable>(Actor.Get());
		if (nullptr != Interactable && Interactable->GetInteractionState(CharacterRef.Get()) == EGInteractionState::Unavailable)
		{
			InteractableCandidates.RemoveAtSwap(Index);
		}
	}

	if (InteractableCandidates.Num() <= 0)
	{
		if (nullptr != InteractableActor)
		{
			GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Interact_Undetect);
		}
		InteractableActor = nullptr;
		return;
	}

	UpdateCandidateScores();
	InteractableCandidates.Sort(&ThisClass::CompareCandidates);

	if (InteractableCandidates.IsEmpty() || InteractableCandidates[0].Score <= -FLT_MAX)
	{
		if (nullptr != InteractableActor)
		{
			GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Interact_Undetect);
		}

		InteractableActor = nullptr;
		return;
	}

	if (InteractableActor != InteractableCandidates[0].Actor)
	{
		InteractableActor = InteractableCandidates[0].Actor;

		IGInteractable* Interactable = Cast<IGInteractable>(InteractableActor);

		if (nullptr != Interactable)
		{
			FGInteract Message(Interactable->GetInteractionID());
			GEVENT_BROADCAST(this, GGameplayTags::EventTag_Interact_Detect, Message);
		}
	}
}

void UGInteractionComponent::UpdateCandidateScores()
{
	for (FGInteractionCandidate& Candidate : InteractableCandidates)
	{
		IGInteractable* Interactable = Cast<IGInteractable>(Candidate.Actor);
		if (nullptr == Interactable)
		{
			continue;
		}

		Candidate.Priority = Interactable->GetPriority();
		Candidate.Score = CalculateScore(Candidate.Actor);
	}
}

bool UGInteractionComponent::CompareCandidates(const FGInteractionCandidate& ACandidate, const FGInteractionCandidate& BCandidate)
{
	// 상위에서 무효화된 액터를 제거했기 때문에 항상 유효한 값을 가진다.
	if (ACandidate.Priority != BCandidate.Priority)
	{
		return ACandidate.Priority < BCandidate.Priority;
	}

	return ACandidate.Score > BCandidate.Score;
}

float UGInteractionComponent::CalculateScore(const TWeakObjectPtr<AActor>& Candidate)
{
	if (false == CharacterRef.IsValid() || false == IsValid(InteractionSphereComponent))
	{
		return -FLT_MAX;
	}

	const FVector ToCandidate = Candidate->GetActorLocation() - CharacterRef->GetActorLocation();
	// N: 내적 (시야각이 가까울수록 1에 가까움)
	const float ViewDot = FVector::DotProduct(CharacterRef->GetControlRotation().Vector(), ToCandidate.GetSafeNormal());

	if (ViewDot < ViewDotThreshold)
	{
		return -FLT_MAX;
	}

	const float Distance = ToCandidate.Size();
	const float InteractionRadius = InteractionSphereComponent->GetScaledSphereRadius();
	// M: 거리 (가까울수록 1에 가까움, InteractionRadius로 정규화)
	const float DistanceScore = FMath::Clamp(1.0f - (Distance / InteractionRadius), 0.0f, 1.0f);
	return ViewDot + DistanceScore;
}

void UGInteractionComponent::OnInteractStarted()
{
	IGInteractable* Interactable = Cast<IGInteractable>(InteractableActor);
	if (nullptr != Interactable)
	{
		Interactable->OnInteractStarted(CharacterRef.Get());

		FGInteractHold Message(Interactable->GetHoldDuration());
		GEVENT_BROADCAST(this, GGameplayTags::EventTag_Interact_Started, Message);
	}
}

void UGInteractionComponent::OnInteractEnded()
{
	IGInteractable* Interactable = Cast<IGInteractable>(InteractableActor);
	if (nullptr != Interactable)
	{
		GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Interact_Ended);

		if (Interactable->CanInteract(CharacterRef.Get()))
		{
			Interactable->Interact(CharacterRef.Get());
		}
	}
}

void UGInteractionComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IGInteractable* Candidate = Cast<IGInteractable>(OtherActor);
	if (nullptr == Candidate)
	{
		return;
	}

	if (Candidate->GetInteractionState(CharacterRef.Get()) == EGInteractionState::Unavailable)
	{
		return;
	}

	for (int32 CandidateIndex = 0; CandidateIndex < InteractableCandidates.Num(); CandidateIndex++)
	{
		if (InteractableCandidates[CandidateIndex].Actor == OtherActor)
		{
			return;
		}
	}

	InteractableCandidates.Add({OtherActor, 0, 0.f});
}

void UGInteractionComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IGInteractable* Candidate = Cast<IGInteractable>(OtherActor);

	if (nullptr != Candidate)
	{
		for (int32 CandidateIndex = 0; CandidateIndex < InteractableCandidates.Num(); CandidateIndex++)
		{
			if (InteractableCandidates[CandidateIndex].Actor == OtherActor)
			{
				InteractableCandidates.RemoveAtSwap(CandidateIndex);
				return;
			}
		}
	}
}
