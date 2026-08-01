#include "Gimmick/GInteractableActor.h"

#include "Character/GCharacter.h"
#include "Component/GInteractionComponent.h"
#include "Data/GGameEnums.h"
#include "Data/Interact/GInteractionAction.h"
#include "Data/Interact/GInteractionCondition.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

AGInteractableActor::AGInteractableActor()
	: Super()
	, InteractionState(EGInteractionState::Available)
{
	InteractPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractPoint"));
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

	UGInteractionComponent* InteractionComp = Character->GetInteractionComponent();
	if (false == IsValid(InteractionComp))
	{
		return;
	}

	FSimpleDelegate OnCompleted = FSimpleDelegate::CreateUObject(this, &ThisClass::OnInteractionCompleted);
	InteractionComp->RunActionPipeline(Actions, this, TargetActor, OnCompleted);
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
	RequestAsyncLoad();
	RequestAsyncLoadActions();
}

void AGInteractableActor::RequestAsyncLoad()
{
	TArray<FSoftObjectPath> SoftPaths;
	for (const TSoftClassPtr<UGInteractionCondition>& ConditionPtr : ConditionClassPtrs)
	{
		SoftPaths.Add(ConditionPtr.ToSoftObjectPath());
	}

	if (SoftPaths.IsEmpty() || false == Conditions.IsEmpty())
	{
		return;
	}

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(SoftPaths, FStreamableDelegate::CreateUObject(this, &ThisClass::OnConditionsLoaded));
}

void AGInteractableActor::OnConditionsLoaded()
{
	for (const TSoftClassPtr<UGInteractionCondition>& ConditionClassPtr : ConditionClassPtrs)
	{
		UClass* ConditionClass = ConditionClassPtr.Get();
		if (nullptr == ConditionClass)
		{
			continue;
		}

		UGInteractionCondition* NewCondition = NewObject<UGInteractionCondition>(this, ConditionClass);
		if (IsValid(NewCondition))
		{
			NewCondition->Init(this);
			Conditions.Add(NewCondition);
		}
	}
}

void AGInteractableActor::RequestAsyncLoadActions()
{
	TArray<FSoftObjectPath> SoftPaths;
	for (const TSoftClassPtr<UGInteractionAction>& ActionPtr : ActionClassPtrs)
	{
		SoftPaths.Add(ActionPtr.ToSoftObjectPath());
	}

	if (SoftPaths.IsEmpty() || false == Actions.IsEmpty())
	{
		return;
	}

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(SoftPaths, FStreamableDelegate::CreateUObject(this, &ThisClass::OnActionsLoaded));
}

void AGInteractableActor::OnActionsLoaded()
{
	for (const TSoftClassPtr<UGInteractionAction>& ActionClassPtr : ActionClassPtrs)
	{
		UClass* ActionClass = ActionClassPtr.Get();
		if (nullptr == ActionClass)
		{
			continue;
		}

		UGInteractionAction* NewAction = NewObject<UGInteractionAction>(this, ActionClass);
		if (IsValid(NewAction))
		{
			Actions.Add(NewAction);
		}
	}
}
