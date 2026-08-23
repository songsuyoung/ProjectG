#pragma once

#include "CoreMinimal.h"
#include "Interface/GMessageReceiver.h"
#include "UI/Base/GCommonActivatableWidget.h"
#include "GInventoryWidget.generated.h"

class UGTileView;
class UGInventoryOptions;
class UGInventoryComponent;
class UGItemEntry;
struct FGItemRow;
enum class EGOptionType : uint8;
UCLASS()
class PROJECTGCLIENT_API UGInventoryWidget : public UGCommonActivatableWidget, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;

protected:
	UGInventoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool NativeOnHandleBackAction() override;
	
protected:
	UGItemEntry* CreateItemEntry(FName ItemID, const FGItemRow* ItemRow, int32 ItemCount);
	
	void AddInventoryUI(FName ItemID, int32 ItemCount);
	void UseInventoryUI(FName ItemID, int32 ItemCount);

	void OnItemClicked(UObject* Object);
	void OnOptionClicked(EGOptionType OptionType);
protected:
	UPROPERTY(EditAnywhere, Category = "Inventory|Settings")
	int32 InitialSlotCount;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UGTileView> TileView_Inventory;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UGInventoryOptions> InventoryOptions;
	
protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Entries;

	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> SelectedObject;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UGInventoryComponent> OwnerInventoryComponent;
};
