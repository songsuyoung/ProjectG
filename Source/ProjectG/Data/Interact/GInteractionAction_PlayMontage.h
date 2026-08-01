// Source/ProjectG/Data/Interact/GInteractionAction_PlayMontage.h
#pragma once

#include "CoreMinimal.h"
#include "Data/Interact/GInteractionAction.h"
#include "GInteractionAction_PlayMontage.generated.h"

class UAnimInstance;
class ACharacter;

UCLASS()
class PROJECTG_API UGInteractionAction_PlayMontage : public UGInteractionAction
{
    GENERATED_BODY()

public:
    virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;

private:
    UFUNCTION()
    void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

    UFUNCTION()
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    void UnbindDelegates();

private:
    UPROPERTY(Transient)
    TWeakObjectPtr<UAnimInstance> AnimInstanceRef;

    UPROPERTY(Transient)
    TWeakObjectPtr<UAnimMontage> MontageRef;

    FName WaitNotifyName = NAME_None;
};
