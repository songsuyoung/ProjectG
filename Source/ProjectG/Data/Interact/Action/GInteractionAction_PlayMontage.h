#pragma once

#include "CoreMinimal.h"
#include "GInteractionAction.h"
#include "GInteractionAction_PlayMontage.generated.h"

class UAnimInstance;
class ACharacter;

UCLASS()
class PROJECTG_API UGInteractionAction_PlayMontage : public UGInteractionAction
{
    GENERATED_BODY()

public:
    virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;

protected:
    UFUNCTION()
    void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

    UFUNCTION()
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    void UnbindDelegates();

protected:
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    FName NotifyName;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> Montage;
    
protected:
    UPROPERTY(Transient)
    TWeakObjectPtr<UAnimInstance> AnimInstanceRef;
};
