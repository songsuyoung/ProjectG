#include "GEventManager.h"

#include "GGameInstance.h"

UGEventManager* UGEventManager::Get(UObject* Object)
{
    UWorld* World = Object->GetWorld();
    check(World);

    UGGameInstance* GameInstance = Cast<UGGameInstance>(World->GetGameInstance());
    check(GameInstance);

    return GameInstance->GetEventManager();
}

void UGEventManager::AddReceiver(FGameplayTag Tag, IGMessageReceiver* Receiver)
{
    Listeners.FindOrAdd(Tag).AddUnique(Receiver);
}

void UGEventManager::RemoveReceiver(FGameplayTag Tag, IGMessageReceiver* Receiver)
{
    if (TArray<TWeakInterfacePtr<IGMessageReceiver>>* Array = Listeners.Find(Tag))
    {
        Array->RemoveSwap(Receiver);
    }
}

void UGEventManager::Broadcast(FGameplayTag Tag, FGMessage* Message)
{
    // Tag 자신 + 부모 태그들 순회 ( Event.Interact.Detect → Event.Interact → Event)
    FGameplayTagContainer TagHierarchy = Tag.GetGameplayTagParents();

    for (const FGameplayTag& CurrentTag : TagHierarchy)
    {
        TArray<TWeakInterfacePtr<IGMessageReceiver>>* Array = Listeners.Find(CurrentTag);
        if (nullptr == Array)
        {
            continue;
        }

        for (int32 i = Array->Num() - 1; i >= 0; --i)
        {
            IGMessageReceiver* Receiver = Cast<IGMessageReceiver>((*Array)[i].Get());
            if (nullptr == Receiver)
            {
                Array->RemoveAtSwap(i);
                continue;
            }
            Receiver->OnMessage(Tag, Message);
        }
    }
}
