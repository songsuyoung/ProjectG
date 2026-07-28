#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Interface/GMessageReceiver.h"
#include "GInteractionInfoWidget.generated.h"

class UTextBlock;
enum class EGMessageType;
struct FGMessage;

UCLASS()
class PROJECTGCLIENT_API UGInteractionInfoWidget : public UCommonUserWidget, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual void OnMessage(EGMessageType Type, FGMessage* Message = nullptr) override;

protected:
	
	void UpdateUI(FName ID);
	
protected:
	
	UPROPERTY(Transient, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Info;
};
