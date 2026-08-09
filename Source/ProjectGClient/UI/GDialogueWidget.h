#pragma once

#include "CoreMinimal.h"
#include "UI/Base/GCommonActivatableWidget.h"
#include "Interface/GMessageReceiver.h"
#include "Input/UIActionBindingHandle.h"
#include "GDialogueWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class UGChoiceButton;
struct FGDialogueChoice;

UCLASS()
class PROJECTGCLIENT_API UGDialogueWidget : public UGCommonActivatableWidget, public IGMessageReceiver
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
protected:
	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;

	void OnBackPressed();
	void OnApplyPressed();
	void OnChoiceClicked(int32 Index);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnChoicesUpdated(const TArray<FGDialogueChoice>& Choices);

protected:
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ApplyInputActionData;
	
	FUIActionBindingHandle BackHandle;
	FUIActionBindingHandle ApplyHandle;
	
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Speaker;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Body;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> VBox_Choice;
	
protected:
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UGChoiceButton>> ChoiceButtons;
};
