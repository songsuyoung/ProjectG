// Source/ProjectG/Component/GInteractionActionPipeline.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GInteractionActionPipeline.generated.h"

class UGInteractionAction;

UCLASS()
class PROJECTG_API UGInteractionActionPipeline : public UObject
{
    GENERATED_BODY()

public:
    void Run(TArray<UGInteractionAction*>& InActions, AActor* InOwnerActor, AActor* InTargetActor, FSimpleDelegate InOnCompleted);
    void Tick(float DeltaTime);
    bool IsRunning() const { return bIsRunning; }

private:
    void RunNextAction();

private:
    UPROPERTY(Transient)
    TArray<TObjectPtr<UGInteractionAction>> Actions;

    TWeakObjectPtr<AActor> OwnerActorRef;

    TWeakObjectPtr<AActor> TargetActorRef;

    FSimpleDelegate OnAllCompleted;
    int32 CurrentIndex = 0;
    bool bIsRunning = false;
};
