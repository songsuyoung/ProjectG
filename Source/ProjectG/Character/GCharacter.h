#pragma once

#include "CoreMinimal.h"
#include "GBaseCharacter.h"
#include "GCharacter.generated.h"

class UGInteractionComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROJECTG_API AGCharacter : public AGBaseCharacter
{
	GENERATED_BODY()

public:
	AGCharacter();
	
	int32 GetLevel() { return Level; }
	bool ContainInventory(FName ItemName) { return Inventory.Contains(ItemName); }
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UGInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
	virtual void BeginPlay() override;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGInteractionComponent> InteractionComponent;
	
	UPROPERTY(EditAnywhere, Category = "Temp")
	int32 Level;
	
	UPROPERTY(EditAnywhere, Category = "Temp|Inventory")
	TArray<FName> Inventory;
};
