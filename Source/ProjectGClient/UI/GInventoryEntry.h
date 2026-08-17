#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "GInventoryEntry.generated.h"

class UImage;
class UTextBlock;
UCLASS()
class UGItemEmptyEntry : public UObject
{
	GENERATED_BODY()
};

UCLASS()
class UGItemEntry : public UGItemEmptyEntry
{
	GENERATED_BODY()

public:
	UGItemEntry();

	UPROPERTY(Transient)
	FName ItemID;

	UPROPERTY(Transient)
	TSoftObjectPtr<UTexture2D> IconImage;

	UPROPERTY(Transient)
	FText ItemName;

	UPROPERTY(Transient)
	int32 Count;

	UPROPERTY(Transient)
	uint8 bLocked : 1;
};

UCLASS()
class PROJECTGCLIENT_API UGInventoryEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	void OnIconLoaded();

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Count;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Lock;
	
	TSharedPtr<FStreamableHandle> StreamableHandle;
};
