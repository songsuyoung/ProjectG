#include "GInteractionAction_PlayMontage.h"

#include "Animation/AnimInstance.h"
#include "Gimmick/GInteractableActor.h"
#include "GameFramework/Character.h"

void UGInteractionAction_PlayMontage::Execute(AActor* OwnerActor, AActor* TargetActor)
{
    ACharacter* Character = Cast<ACharacter>(TargetActor);
    if (false == IsValid(Character))
    {
        Finish();
        return;
    }

    AGInteractableActor* Interactable = Cast<AGInteractableActor>(OwnerActor);
    if (false == IsValid(Interactable))
    {
        Finish();
        return;
    }

    UAnimMontage* Montage = Interactable->GetInteractMontage();
    if (false == IsValid(Montage))
    {
        Finish();
        return;
    }

    UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
    if (false == IsValid(AnimInstance))
    {
        Finish();
        return;
    }

    AnimInstanceRef = AnimInstance;
    MontageRef = Montage;
    WaitNotifyName = Interactable->GetInteractNotifyName();

    Character->PlayAnimMontage(Montage);

    if (false == WaitNotifyName.IsNone())
    {
        AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::OnNotifyBegin);
    }
    else
    {
        AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnMontageEnded);
    }
}

void UGInteractionAction_PlayMontage::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    if (NotifyName != WaitNotifyName)
    {
        return;
    }

    UnbindDelegates();
    Finish();
}

void UGInteractionAction_PlayMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage != MontageRef.Get())
    {
        return;
    }

    UnbindDelegates();
    Finish();
}

void UGInteractionAction_PlayMontage::UnbindDelegates()
{
    if (false == AnimInstanceRef.IsValid())
    {
        return;
    }

    AnimInstanceRef->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ThisClass::OnNotifyBegin);
    AnimInstanceRef->OnMontageEnded.RemoveDynamic(this, &ThisClass::OnMontageEnded);
}
