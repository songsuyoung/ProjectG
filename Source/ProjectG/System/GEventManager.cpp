#include "GEventManager.h"

// Newly Created Files...
#include "GEventManager.h"

#include "GGameInstance.h"

UGEventManager* UGEventManager::Get(UObject* Object)
{
    // UObject�� World �� ������ ���� �ʴ�.
    // �׷��⶧���� �ܺ� ���ͷκ��� World�� �����;��Ѵ�.
    UWorld* World = Object->GetWorld();

    check(World);

    UGGameInstance* GameInstance = Cast<UGGameInstance>(World->GetGameInstance());

    check(GameInstance);

    return GameInstance->GetEventManager();
}

void UGEventManager::AddReceiver(IGMessageReceiver* Receiver)
{
    Receivers.AddTail(Receiver);
}

void UGEventManager::RemoveReceiver(IGMessageReceiver* Receiver)
{
    Receivers.RemoveNode(Receiver);
}

void UGEventManager::Notify(EGMessageType Type, FGMessage* Message)
{
    auto* NextReceiver = Receivers.GetHead();

    while (nullptr != NextReceiver)
    {
        // GetValue -> UObject�� ����.
        IGMessageReceiver* Receiver = Cast<IGMessageReceiver>(NextReceiver->GetValue().Get());

        if (nullptr == Receiver)
        {
            Receivers.RemoveNode(Receiver);
            NextReceiver = NextReceiver->GetNextNode();
            continue;
        }

        // �޽��� ȣ��
        Receiver->OnMessage(Type, Message);
        NextReceiver = NextReceiver->GetNextNode();
    }
}