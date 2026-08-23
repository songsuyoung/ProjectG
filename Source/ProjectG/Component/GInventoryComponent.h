#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTG_API UGInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGInventoryComponent();

	bool CanAcquire(FName ItemID, int32 Count = 1);
	void Acquire(FName PromptID, int32 Count = 1);
	void UseItem(FName ItemName, int32 Count);
	bool CanUseItem(FName ItemName, int32 Count);
	int32 GetItemCount(FName ItemName);
	const TMap<FName, int32>& GetInventorySlots() const { return InventorySlots; }

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FString> GetInventoryLog();
	
protected:
	
	UPROPERTY(Transient)
	TMap<FName, int32> InventorySlots;
};
