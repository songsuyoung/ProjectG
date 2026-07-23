#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GHeroComponent.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue
;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTG_API UGHeroComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGHeroComponent();

	void SetupInputComponent();
	
protected:
	virtual void BeginPlay() override;
	
protected:
	
	void Initalize();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookInputAction;
protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<APlayerController> PlayerControllerRef;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<ACharacter> CharacterRef;
};
