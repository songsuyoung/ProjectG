#pragma once

#include "CoreMinimal.h"
#include "GBaseCharacter.h"
#include "GCharacter.generated.h"

UCLASS()
class PROJECTG_API AGCharacter : public AGBaseCharacter
{
	GENERATED_BODY()

public:
	AGCharacter();
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
};
