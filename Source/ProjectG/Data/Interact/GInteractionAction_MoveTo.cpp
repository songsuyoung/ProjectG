// Source/ProjectG/Data/Interact/GInteractionAction_MoveTo.cpp
#include "Data/Interact/GInteractionAction_MoveTo.h"

#include "Gimmick/GInteractableActor.h"
#include "GameFramework/Character.h"

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
    TargetRotation = InteractTransform.Rotator();
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

    FRotator CurrentRotation = CharacterRef->GetActorRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);
    CharacterRef->SetActorRotation(NewRotation);

    float DistanceXY = FVector::DistXY(CurrentLocation, TargetLocation);
    if (DistanceXY <= ArrivalThreshold)
    {
        Finish();
    }
}
