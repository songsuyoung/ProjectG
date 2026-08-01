#include "GInteractionActionComponent.h"

#include "Data/Interact/Action/GInteractionAction.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

UGInteractionActionComponent::UGInteractionActionComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UGInteractionActionComponent::Init()
{
    Actions.Reset();

    TArray<FSoftObjectPath> SoftPaths;
    for (const TSoftClassPtr<UGInteractionAction>& ActionPtr : ActionClassPtrs)
    {
        SoftPaths.Add(ActionPtr.ToSoftObjectPath());
    }

    if (SoftPaths.IsEmpty())
    {
        return;
    }

    TWeakObjectPtr<UGInteractionActionComponent> WeakThis = this;
    FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
    StreamableManager.RequestAsyncLoad(SoftPaths, FStreamableDelegate::CreateLambda([WeakThis]()
    {
        if (false == WeakThis.IsValid())
        {
            return;
        }

        WeakThis->OnActionsLoaded();
    }));
}

void UGInteractionActionComponent::OnActionsLoaded()
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

void UGInteractionActionComponent::Run(AActor* InOwnerActor, AActor* InTargetActor, FSimpleDelegate InOnCompleted)
{
    OwnerActorRef = InOwnerActor;
    TargetActorRef = InTargetActor;
    OnAllCompleted = InOnCompleted;
    CurrentIndex = 0;
    bIsRunning = true;

    RunNextAction();
}

void UGInteractionActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (false == bIsRunning)
    {
        return;
    }

    int32 ActiveIndex = CurrentIndex - 1;
    if (ActiveIndex >= 0 && ActiveIndex < Actions.Num())
    {
        Actions[ActiveIndex]->Tick(DeltaTime);
    }
}

void UGInteractionActionComponent::RunNextAction()
{
    if (CurrentIndex >= Actions.Num())
    {
        bIsRunning = false;
        OnAllCompleted.ExecuteIfBound();
        return;
    }

    UGInteractionAction* Action = Actions[CurrentIndex++];
    if (nullptr == Action)
    {
        RunNextAction();
        return;
    }

    Action->OnCompleted.BindUObject(this, &ThisClass::RunNextAction);
    Action->Execute(OwnerActorRef.Get(), TargetActorRef.Get());
}
