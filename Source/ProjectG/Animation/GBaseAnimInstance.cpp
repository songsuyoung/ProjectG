#include "Animation/GBaseAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	
	if (OwnerCharacter.IsValid())
	{
		CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UGBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (false == CharacterMovementComponent.IsValid() || false == OwnerCharacter.IsValid())
	{
		return;
	}
	
	Speed = CharacterMovementComponent->Velocity.Size2D();
	Velocity = CharacterMovementComponent->Velocity;
	bIsFalling = CharacterMovementComponent->IsFalling();
}
