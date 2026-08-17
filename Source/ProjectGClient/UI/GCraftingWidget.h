#pragma once

#include "CoreMinimal.h"
#include "Interface/GMessageReceiver.h"
#include "UI/Base/GCommonActivatableWidget.h"
#include "GCraftingWidget.generated.h"

struct FGWorkbenchIngredient;

USTRUCT()
struct FGWorkbenchIngredientEntry 
{        
	GENERATED_BODY()
public:
	
	UPROPERTY(Transient)
	TArray<FGWorkbenchIngredient> Ingredients;                                                                                                                                                                                    
};   

class UGTileView;
class UGInventoryComponent;
class UCommonButtonBase;
UCLASS()
class PROJECTGCLIENT_API UGCraftingWidget : public UGCommonActivatableWidget, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;

protected:
	UGCraftingWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;

protected:
	
	void Init();
private:
	void OnClicked();
	void OnWorkbenchItemSelected(UObject* Item);
	virtual bool NativeOnHandleBackAction() override;
	
protected:
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UGTileView> TileView_Workbench;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UGTileView> TileView_Ingredients;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonButtonBase> Button_Maker;
	
	UPROPERTY(EditAnywhere)
	int32 MaxSlots;
	
protected:
	
	UPROPERTY(Transient)
	FName CurrentItem;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UGInventoryComponent> InventoryComponent;
	
	UPROPERTY(Transient)
	TMap<FName, FGWorkbenchIngredientEntry> WorkbenchIngredientEntry;
};
