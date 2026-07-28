#pragma once

#include "CoreMinimal.h"
#include "Interface/GMessageReceiver.h"
#include "Containers/List.h"
#include "GEventManager.generated.h"

enum class EGMessageType;
struct FGMessage;

// EventManager - Subject
UCLASS()
class PROJECTG_API UGEventManager : public UObject
{
	GENERATED_BODY()

public:

	static UGEventManager* Get(UObject* Context);
	
	void AddReceiver(IGMessageReceiver* Receiver);
	void RemoveReceiver(IGMessageReceiver* Receiver);
	void Notify(EGMessageType Type, FGMessage* Message = nullptr);
protected:

	TDoubleLinkedList<TWeakInterfacePtr<IGMessageReceiver>> Receivers;
};
