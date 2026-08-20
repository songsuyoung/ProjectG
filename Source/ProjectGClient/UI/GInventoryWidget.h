#pragma once

#include "CoreMinimal.h"
#include "Interface/GMessageReceiver.h"
#include "UI/Base/GCommonActivatableWidget.h"
#include "GInventoryWidget.generated.h"

class UGTileView;

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
	
	void AddInventoryUI(FName ItemID, int32 ItemCount);
	void UseInventoryUI(FName ItemID, int32 ItemCount);

	void OnItemClicked(UObject* Object);
protected:
	UPROPERTY(EditAnywhere, Category = "Inventory|Settings")
	int32 InitialSlotCount;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Entries;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UGTileView> TileView_Inventory;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UUserWidget> WBP_Options;
};
