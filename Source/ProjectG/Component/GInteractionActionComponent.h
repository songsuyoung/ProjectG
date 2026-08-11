#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GInteractionActionComponent.generated.h"

class UGInteractionCondition;
class UGInteractionAction;
UCLASS(Blueprintable)
class PROJECTG_API UGInteractionActionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGInteractionActionComponent(const FObjectInitializer& ObjectInitializer);

    void Run(AActor* InOwnerActor, AActor* InTargetActor, FSimpleDelegate InOnCompleted);
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    bool IsRunning() const { return bIsRunning; }
    const TArray<FName>& GetConditions() { return ConditionIDs;}
protected:
    
    void RunNextAction();

protected:
    UPROPERTY(EditAnywhere, Instanced, Category = "Interaction|Actions")
    TArray<TObjectPtr<UGInteractionAction>> Actions;
    
    UPROPERTY(EditAnywhere, Category = "Interaction")
    TArray<FName> ConditionIDs;;
protected:
    UPROPERTY(Transient)
    TWeakObjectPtr<AActor> OwnerActorRef;

    UPROPERTY(Transient)
    TWeakObjectPtr<AActor> TargetActorRef;

    FSimpleDelegate OnAllCompleted;
    int32 CurrentIndex = 0;
    bool bIsRunning = false;
};
