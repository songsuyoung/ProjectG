#include "Gimmick/GInteractableActor.h"

#include "Data/GGameEnums.h"
#include "Data/Interact/GInteractionCondition.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

AGInteractableActor::AGInteractableActor()
	: Super()
	, InteractionState(EGInteractionState::Available)
{
	
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
	InternalInteract(TargetActor);
	InteractionState = EGInteractionState::Unavailable;
}

void AGInteractableActor::InternalInteract(AActor* TargetActor)
{
	for (UGInteractionCondition* Condition : Conditions)
	{
		Condition->Apply(TargetActor);
	}
}

void AGInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	RequestAsyncLoad();
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
		if (ConditionClass == nullptr)
		{
			continue;
		}

		UGInteractionCondition* NewCondition = NewObject<UGInteractionCondition>(this, ConditionClass);
		if (NewCondition != nullptr)
		{
			NewCondition->Init(this);
			Conditions.Add(NewCondition);
		}
	}
}
