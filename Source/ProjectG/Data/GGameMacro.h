#pragma once
#include "CoreMinimal.h"

#define GEVENT_MESSAGE_NOTIFY(Context, Type) \
	UGEventManager::Get(Context)->Notify(Type, nullptr); \

#define GEVENT_MESSAGE_NOTIFY_MSG(Context, Type, Msg) \
	UGEventManager::Get(Context)->Notify(Type, &Msg);

#define GEVENT_MESSAGE_ADD(Context, Receiver)	\
	UGEventManager::Get(Context)->AddReceiver(Receiver) \

#define GEVENT_MESSAGE_REMOVE(Context, Receiver)	\
	UGEventManager::Get(Context)->RemoveReceiver(Receiver) \
