#include "Interface/GInteractable.h"

#include "Character/GCharacter.h"
#include "Component/GInteractionActionComponent.h"
#include "System/GConditionManager.h"

EGInteractionState IGInteractable::GetInteractionState(AActor* TargetActor)
{
	FGInteractionSharedState& SharedState = GetInteractionSharedState();

	if (SharedState.State == EGInteractionState::Unavailable)
	{
		return SharedState.State;
	}
	
	UGInteractionActionComponent* ActionComp = GetInteractionActionComponent();
	
	if (false == IsValid(ActionComp))
	{
		return EGInteractionState::None;
	}

	UGConditionManager* ConditionManager = UGConditionManager::Get(TargetActor);
	check(ConditionManager);
	for (const FName& ConditionID : ActionComp->GetConditions())
	{
		if (false == ConditionManager->IsSatisfied(ConditionID, TargetActor))
		{	
			SharedState.State = EGInteractionState::Pending;
			return SharedState.State;
		}
	}
	
	SharedState.State = EGInteractionState::Available;
	return SharedState.State;
}

void IGInteractable::OnInteractStarted(AActor* TargetActor)
{
	FGInteractionSharedState& SharedState = GetInteractionSharedState();

	UWorld* World = CastChecked<AActor>(this)->GetWorld();
	check(World);

	SharedState.StartTimestamp = World->GetTimeSeconds();
}

bool IGInteractable::CanInteract(AActor* TargetActor)
{
	UWorld* World = CastChecked<AActor>(this)->GetWorld();
	check(World);

	float Duration = World->GetTimeSeconds() - GetInteractionSharedState().StartTimestamp;
	return Duration >= GetHoldDuration() && GetInteractionState(TargetActor) == EGInteractionState::Available;
}

void IGInteractable::Interact(AActor* TargetActor)
{
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);
	if (false == IsValid(Character))
	{
		return;
	}

	UGInteractionActionComponent* ActionComp = GetInteractionActionComponent();
	if (false == IsValid(ActionComp))
	{
		return;
	}

	FGInteractionSharedState& SharedState = GetInteractionSharedState();
	SharedState.InteractingCharacter = Character;

	AActor* SelfActor = CastChecked<AActor>(this);
	FSimpleDelegate OnCompleted = FSimpleDelegate::CreateWeakLambda(SelfActor, [this]()
	{
		FGInteractionSharedState& State = GetInteractionSharedState();
		State.State = IsRepeatable() ? EGInteractionState::Available : EGInteractionState::Unavailable;
		State.InteractingCharacter = nullptr;
	});

	ActionComp->Run(SelfActor, TargetActor, OnCompleted);
}
