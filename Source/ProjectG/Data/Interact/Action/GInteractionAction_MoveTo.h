
#pragma once

#include "CoreMinimal.h"
#include "Data/Interact/Action/GInteractionAction.h"
#include "GInteractionAction_MoveTo.generated.h"

class ACharacter;

UCLASS()
class PROJECTG_API UGInteractionAction_MoveTo : public UGInteractionAction
{
    GENERATED_BODY()

public:
    virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void Finish() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "MoveTo")
    float MoveInterpSpeed = 5.f;

    UPROPERTY(EditDefaultsOnly, Category = "MoveTo")
    float RotationInterpSpeed = 10.f;

    UPROPERTY(EditDefaultsOnly, Category = "MoveTo")
    float ArrivalThreshold = 30.f;
	
protected:
    UPROPERTY(Transient)
    TWeakObjectPtr<ACharacter> CharacterRef;

    UPROPERTY(Transient)
    FVector TargetLocation;
    
    UPROPERTY(Transient)
    FRotator TargetRotation;
};
