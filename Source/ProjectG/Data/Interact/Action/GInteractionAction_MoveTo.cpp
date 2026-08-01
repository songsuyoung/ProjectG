#include "GInteractionAction_MoveTo.h"

#include "Gimmick/GInteractableActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGInteractionAction_MoveTo::Execute(AActor* OwnerActor, AActor* TargetActor)
{
    CharacterRef = Cast<ACharacter>(TargetActor);
    if (false == CharacterRef.IsValid())
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

    FTransform InteractTransform = Interactable->GetInteractPointTransform();
    TargetLocation = InteractTransform.GetLocation();

    FVector DirectionToOwner = (OwnerActor->GetActorLocation() - TargetLocation).GetSafeNormal();
    TargetRotation = DirectionToOwner.Rotation();
    TargetRotation.Roll = TargetRotation.Pitch = 0.0f;
}

void UGInteractionAction_MoveTo::Tick(float DeltaTime)
{
    if (false == CharacterRef.IsValid())
    {
        Finish();
        return;
    }

    FVector CurrentLocation = CharacterRef->GetActorLocation();
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MoveInterpSpeed);
    CharacterRef->SetActorLocation(NewLocation);

    AController* Controller = CharacterRef->GetController();
    if (IsValid(Controller))
    {
        FRotator CurrentRotation = Controller->GetControlRotation();
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);
        Controller->SetControlRotation(NewRotation);
    }

    float DistanceXY = FVector::DistXY(CurrentLocation, TargetLocation);
    if (DistanceXY <= ArrivalThreshold)
    {
        Finish();
    }
}

void UGInteractionAction_MoveTo::Finish()
{
    Super::Finish();
}
