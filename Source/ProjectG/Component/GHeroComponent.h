#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GHeroComponent.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class AGCharacter;

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

	void OnInteractStarted();
	void OnInteractEnded();

	void OnInventoryToggle();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InventoryInputAction;
protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<APlayerController> PlayerControllerRef;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<AGCharacter> CharacterRef;
};
