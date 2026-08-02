#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "GMessageReceiver.generated.h"

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

	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) = 0;
};
