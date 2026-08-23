#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GInventoryOptions.generated.h"

UENUM()
enum class EGOptionType : uint8
{
	None,
	Use,
	Keep,
};
class UCommonButtonBase;
class UVerticalBox;
UCLASS()
class PROJECTGCLIENT_API UGInventoryOptions : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UGInventoryOptions(const FObjectInitializer& ObjectInitializer);
	
	DECLARE_DELEGATE_OneParam(FOnClickedOption, EGOptionType);
	FOnClickedOption OnClickedOption;
protected:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
protected:
	
	void OnClicked(int Index);
	
protected:
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> VBox_Option;
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UCommonButtonBase>> Button_Options;
	
};
