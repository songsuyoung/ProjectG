#pragma once

#include "CoreMinimal.h"
#include "Interface/GMessageReceiver.h"
#include "UI/Base/GCommonActivatableWidget.h"
#include "GCraftingWidget.generated.h"

class UGTileView;

UCLASS()
class PROJECTGCLIENT_API UGCraftingWidget : public UGCommonActivatableWidget, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool NativeOnHandleBackAction() override;
};
