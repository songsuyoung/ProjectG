#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Interface/GMessageReceiver.h"
#include "GEventManager.generated.h"

struct FGMessage;

UCLASS()
class PROJECTG_API UGEventManager : public UObject
{
	GENERATED_BODY()

public:

	static UGEventManager* Get(UObject* Context);

	void AddReceiver(FGameplayTag Tag, IGMessageReceiver* Receiver);
	void RemoveReceiver(FGameplayTag Tag, IGMessageReceiver* Receiver);
	void Broadcast(FGameplayTag Tag, FGMessage* Message = nullptr);

private:

	TMap<FGameplayTag, TArray<TWeakInterfacePtr<IGMessageReceiver>>> Listeners;
};
