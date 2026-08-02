#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GWidgetFactory.generated.h"

class UUserWidget;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class PROJECTGCLIENT_API UGWidgetFactory : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<UUserWidget> FindWidgetClassForData(const UObject* Data) const;
};

UCLASS()
class PROJECTGCLIENT_API UGWidgetFactory_Class : public UGWidgetFactory
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<UUserWidget> FindWidgetClassForData_Implementation(const UObject* Data) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Factory")
	TMap<TSoftClassPtr<UObject>, TSubclassOf<UUserWidget>> EntryWidgetForClass;
};
