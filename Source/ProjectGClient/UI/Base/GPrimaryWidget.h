#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "GPrimaryWidget.generated.h"

class UGCommonActivatableWidget;
class UCommonActivatableWidgetStack;
class UGUIWindowLoadMethod;

USTRUCT()
struct FGPoolContainer
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGCommonActivatableWidget>> Container;
};

UCLASS()
class PROJECTGCLIENT_API UGPrimaryWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	virtual UGCommonActivatableWidget* GetOrCreateInstance(UClass* Class, const FGameplayTag& LayerTag);
	void ReturnToPool(UGCommonActivatableWidget* Widget);

protected:

	UGCommonActivatableWidget* GetOrCreateInstanceInternal(UClass* Class, const FGameplayTag& LayerTag);

protected:

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UGUIWindowLoadMethod> WindowLoadMethodClass;
	
protected:
	
	UPROPERTY(Transient, meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> Root;
	
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetStack>> LayerMap;

	UPROPERTY(Transient)
	TMap<FName, FGPoolContainer> PoolingContainer;
};
