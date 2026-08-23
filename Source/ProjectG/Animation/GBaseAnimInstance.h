#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GBaseAnimInstance.generated.h"

class UCharacterMovementComponent;
class ACharacter;
UCLASS()
class PROJECTG_API UGBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	FVector Velocity;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	float Speed;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	uint8 bIsFalling : 1;
};
