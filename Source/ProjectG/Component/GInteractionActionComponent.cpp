#include "GInteractionActionComponent.h"

#include "Data/Condition/Interact/GInteractionCondition.h"
#include "Data/Interact/Action/GInteractionAction.h"

UGInteractionActionComponent::UGInteractionActionComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryComponentTick.bCanEverTick = true;
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

void UGInteractionActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
