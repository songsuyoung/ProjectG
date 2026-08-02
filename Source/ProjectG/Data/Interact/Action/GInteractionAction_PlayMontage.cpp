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
    
    UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
    if (false == IsValid(AnimInstance))
    {
        Finish();
        return;
    }

    AnimInstanceRef = AnimInstance;

    Character->PlayAnimMontage(Montage);

    if (false == NotifyName.IsNone())
    {
        AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::OnNotifyBegin);
    }
    else
    {
        AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnMontageEnded);
    }
}

void UGInteractionAction_PlayMontage::OnNotifyBegin(FName InNotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    if (NotifyName != InNotifyName)
    {
        return;
    }

    UnbindDelegates();
    Finish();
}

void UGInteractionAction_PlayMontage::OnMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
{
    if (Montage != InMontage)
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
