#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "GInventoryEntry.h"
#include "GWorkbenchEntry.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PROJECTGCLIENT_API UGWorkbenchEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void OnIconLoaded();
	
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Image_Icon;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Name;

protected:
	TSharedPtr<FStreamableHandle> StreamableHandle;
};
