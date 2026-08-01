#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GInteractionActionComponent.generated.h"

class UGInteractionAction;

UCLASS(Blueprintable, EditInlineNew)
class PROJECTG_API UGInteractionActionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGInteractionActionComponent(const FObjectInitializer& ObjectInitializer);
    
    void Init();
    void Run(AActor* InOwnerActor, AActor* InTargetActor, FSimpleDelegate InOnCompleted);
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    bool IsRunning() const { return bIsRunning; }

private:
    void OnActionsLoaded();
    void RunNextAction();

protected:    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Actions")
    TArray<TSoftClassPtr<UGInteractionAction>> ActionClassPtrs;

protected:
    UPROPERTY(Transient)
    TArray<TObjectPtr<UGInteractionAction>> Actions;

    UPROPERTY(Transient)
    TWeakObjectPtr<AActor> OwnerActorRef;

    UPROPERTY(Transient)
    TWeakObjectPtr<AActor> TargetActorRef;

    FSimpleDelegate OnAllCompleted;
    int32 CurrentIndex = 0;
    bool bIsRunning = false;
};
