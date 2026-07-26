#include "Gimmick/GInteractableActor.h"

#include "Data/GGameEnums.h"
#include "Data/Interact/GInteractionCondition.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

AGInteractableActor::AGInteractableActor()
	: Super()
	, InteractionState(EGInteractionState::Available)
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComponent;
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

bool AGInteractableActor::CanInteract(AActor* TargetActor)
{
	return GetInteractionState(TargetActor) == EGInteractionState::Available;
}

void AGInteractableActor::Interact(AActor* TargetActor)
{
	// 접촉 후 3초 후에 사라진다.
	SetLifeSpan(3.0f);
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
