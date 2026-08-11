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

	bool CanAcquire(FName ItemID);
	void Acquire(FName PromptID);
	void UseItem(FName ItemName, int32 Count);
	bool CanUseItem(FName ItemName, int32 Count);
	const TMap<FName, int32>& GetInventorySlots() const { return InventorySlots; }
	
protected:
	
	UPROPERTY(Transient)
	TMap<FName, int32> InventorySlots;
};
