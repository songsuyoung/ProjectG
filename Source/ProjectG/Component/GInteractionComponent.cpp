#include "Component/GInteractionComponent.h"

// UE 5.
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Interface/GInteractable.h"

UGInteractionComponent::UGInteractionComponent()
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
	
	if (false == InteractableCandidates.IsEmpty())
	{
		UpdateFocusTarget();
	}
	else
	{
		InteractableActor = nullptr;
	}
}

void UGInteractionComponent::UpdateFocusTarget()
{
	// 무효화된 액터가 있는지 확인한다.
	// 다른 플레이어에 의해 삭제되었을 수도 있음.
	for (int32 Index = InteractableCandidates.Num() - 1; Index >= 0; --Index)
	{
		if (false == InteractableCandidates[Index].IsValid())
		{
			InteractableCandidates.RemoveAtSwap(Index);
		}
	}
	
	TWeakObjectPtr<UGInteractionComponent> ThisWeakPtr = this;
	
	InteractableCandidates.Sort([ThisWeakPtr](const TWeakObjectPtr<AActor>& A, const TWeakObjectPtr<AActor>& B)
	{
		if (false == ThisWeakPtr.IsValid())
		{
			return false;
		}
		
		IGInteractable* ACandidate = Cast<IGInteractable>(A.Get());
		IGInteractable* BCandidate = Cast<IGInteractable>(B.Get());
		
		if (nullptr == ACandidate || nullptr == BCandidate)
		{
			return false;
		}
		
		// 상위에서 무효화된 액터를 제거했기 때문에 항상 유효한 값을 가진다.
		if (ACandidate->GetPriority() != BCandidate->GetPriority())
		{
			return ACandidate->GetPriority() > BCandidate->GetPriority();
		}
		
		return ThisWeakPtr->CalculateScore(A) > ThisWeakPtr->CalculateScore(B);
	});
	
	if (InteractableCandidates.IsValidIndex(0) && InteractableActor !=  InteractableCandidates[0])
	{
		InteractableActor = InteractableCandidates[0];
	}
	else
	{
		InteractableActor = nullptr;
	}
}

float UGInteractionComponent::CalculateScore(const TWeakObjectPtr<AActor>& Candidate)
{
	if (false == CharacterRef.IsValid() || false == IsValid(InteractionSphereComponent))
	{
		return -FLT_MAX;
	}
	
	const FVector ToCandidate = Candidate->GetActorLocation() - CharacterRef->GetActorLocation();
	const float Distance = ToCandidate.Size();
	const float InteractionRadius =  InteractionSphereComponent->GetScaledSphereRadius();

	// N: 내적 (시야각이 가까울수록 1에 가까움)
	const float ViewDot = FVector::DotProduct(CharacterRef->GetControlRotation().Vector(), ToCandidate.GetSafeNormal());
	
	// M: 거리 (가까울수록 1에 가까움, InteractionRadius로 정규화)
	const float DistanceScore = FMath::Clamp(1.0f - (Distance / InteractionRadius), 0.0f, 1.0f);
			
	return  ViewDot + DistanceScore;
}

void UGInteractionComponent::Interact()
{
	if (false == InteractableActor.IsValid() || false == CharacterRef.IsValid())
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Interat! TargetActor = %s"), *InteractableActor->GetActorLabel());
	
	IGInteractable* Interactable = Cast<IGInteractable>(InteractableActor);
	if (nullptr != Interactable)
	{
		Interactable->Interact(CharacterRef.Get());
	}
}

void UGInteractionComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	IGInteractable* Candidate = Cast<IGInteractable>(OtherActor);
	if (Candidate == nullptr)
	{
		return;
	}

	if (MaxInteractableCandidates <= InteractableCandidates.Num())
	{
		return;
	}
	
	InteractableCandidates.AddUnique(OtherActor);
}

void UGInteractionComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IGInteractable* Candidate = Cast<IGInteractable>(OtherActor);
	
	if (Candidate != nullptr)
	{
		InteractableCandidates.Remove(OtherActor);
	}
}
