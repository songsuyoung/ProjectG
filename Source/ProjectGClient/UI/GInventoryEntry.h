#pragma once

#include "CoreMinimal.h"
#include "Interface/GMessageReceiver.h"
#include "UI/Base/GCommonActivatableWidget.h"
#include "GInventoryWidget.generated.h"

class UTileView;
UCLASS()
class PROJECTGCLIENT_API UGInventoryWidget : public UGCommonActivatableWidget, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	virtual void OnMessage(EGMessageType Type, FGMessage* Message) override;
	
protected:
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTileView> TileView_Inventory;
};
