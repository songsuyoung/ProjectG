#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GMessageReceiver.generated.h"

enum class EGMessageType;
struct FGMessage;
UINTERFACE(MinimalAPI)
class UGMessageReceiver : public UInterface
{
	GENERATED_BODY()
};


class PROJECTG_API IGMessageReceiver
{
	GENERATED_BODY()

public:

	virtual void OnMessage(EGMessageType Type, FGMessage* Message = nullptr) = 0;
};