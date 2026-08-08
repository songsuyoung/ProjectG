#pragma once

#include "CoreMinimal.h"
#include "UI/Base/GCommonActivatableWidget.h"
#include "Interface/GMessageReceiver.h"
#include "Input/UIActionBindingHandle.h"
#include "GDialogueWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class UGCommonButtonBase;
struct FGDialogueChoice;

UCLASS()
class PROJECTGCLIENT_API UGDialogueWidget : public UGCommonActivatableWidget, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;

	// Blueprint에서 선택지 버튼 클릭 시 호출
	UFUNCTION(BlueprintCallable)
	void SelectChoice(int32 Index);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
protected:
	void OnBackPressed();
	void OnApplyPressed();
	
	void OnClick_Choice(int32 Index);
	
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
	TArray<TObjectPtr<UGCommonButtonBase>> ChoiceButtons;
};
