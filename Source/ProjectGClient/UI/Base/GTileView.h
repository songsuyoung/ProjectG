#pragma once

#include "CoreMinimal.h"
#include "Components/TileView.h"
#include "GTileView.generated.h"

class UGWidgetFactory;

UCLASS()
class PROJECTGCLIENT_API UGTileView : public UTileView
{
	GENERATED_BODY()

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category = "Factory")
	TArray<TObjectPtr<UGWidgetFactory>> FactoryRules;
};
