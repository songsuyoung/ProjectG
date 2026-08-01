// Source/ProjectG/Component/GInteractionActionPipeline.cpp
#include "Component/GInteractionActionPipeline.h"
#include "Data/Interact/GInteractionAction.h"

void UGInteractionActionPipeline::Run(TArray<UGInteractionAction*>& InActions, AActor* InOwnerActor, AActor* InTargetActor, FSimpleDelegate InOnCompleted)
{
    Actions.Reset();
    for (UGInteractionAction* Action : InActions)
    {
        Actions.Add(Action);
    }

    OwnerActorRef = InOwnerActor;
    TargetActorRef = InTargetActor;
    OnAllCompleted = InOnCompleted;
    CurrentIndex = 0;
    bIsRunning = true;

    RunNextAction();
}

void UGInteractionActionPipeline::Tick(float DeltaTime)
{
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

void UGInteractionActionPipeline::RunNextAction()
{
    if (CurrentIndex >= Actions.Num())
    {
        bIsRunning = false;
        OnAllCompleted.ExecuteIfBound();
        return;
    }

    UGInteractionAction* Action = Actions[CurrentIndex++];
    Action->OnCompleted.BindUObject(this, &ThisClass::RunNextAction);
    Action->Execute(OwnerActorRef.Get(), TargetActorRef.Get());
}
