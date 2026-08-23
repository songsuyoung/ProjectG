#pragma once

#include "CoreMinimal.h"
#include "GBaseCharacter.h"
#include "GCharacter.generated.h"

class UGInteractionComponent;
class UGInventoryComponent;
class USpringArmComponent;
class UGQuestComponent;
class UCameraComponent;

UCLASS()
class PROJECTG_API AGCharacter : public AGBaseCharacter
{
	GENERATED_BODY()

public:
	AGCharacter();
	
	int32 GetLevel() { return Level; }

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UGInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
	FORCEINLINE UGInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE UGQuestComponent* GetQuestComponent() const { return QuestComponent; }

	void AddCompletedDialogue(FName DialogueID) { CompletedDialogueIDs.Add(DialogueID); }
	bool HasCompletedDialogue(FName DialogueID) const { return CompletedDialogueIDs.Contains(DialogueID); }

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void BP_UseItem(FName ItemName, int32 Count);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
  	TObjectPtr<UGInteractionComponent> InteractionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGInventoryComponent> InventoryComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGQuestComponent> QuestComponent;
	
	UPROPERTY(EditAnywhere, Category = "Temp")
	int32 Level;

	UPROPERTY()
	TSet<FName> CompletedDialogueIDs;
};
