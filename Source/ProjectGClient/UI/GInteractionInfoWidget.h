#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Interface/GMessageReceiver.h"
#include "GInteractionInfoWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class PROJECTGCLIENT_API UGInteractionInfoWidget : public UCommonUserWidget, public IGMessageReceiver
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;

protected:

	void UpdateUI(FName ID);

protected:

	UPROPERTY(Transient, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Info;

	UPROPERTY(Transient, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> ProgressBar_Hold;

private:

	bool bIsHolding = false;
	float HoldDuration = 0.f;
	float HoldElapsed = 0.f;
};
