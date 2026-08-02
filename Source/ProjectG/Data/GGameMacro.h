#pragma once
#include "CoreMinimal.h"

#define GEVENT_BROADCAST(Context, Tag, Msg)   UGEventManager::Get(Context)->Broadcast(Tag, &Msg)
#define GEVENT_BROADCAST_EMPTY(Context, Tag)  UGEventManager::Get(Context)->Broadcast(Tag)
#define GEVENT_ADD(Context, Tag, Receiver)    UGEventManager::Get(Context)->AddReceiver(Tag, Receiver)
#define GEVENT_REMOVE(Context, Tag, Receiver) UGEventManager::Get(Context)->RemoveReceiver(Tag, Receiver)
