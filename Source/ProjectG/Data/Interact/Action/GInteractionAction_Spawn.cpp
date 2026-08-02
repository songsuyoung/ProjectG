#include "GInteractionAction_Spawn.h"

#include "Data/GItemRow.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Gimmick/GInteractableActor.h"
#include "System/GDataManager.h"

void UGInteractionAction_Spawn::Execute(AActor* OwnerActor, AActor* TargetActor)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	FGItemRow* ItemRow = DataManager->GetDataTableRow<FGItemRow>(EGDataTableType::Item, SpawnID);

	if (nullptr == ItemRow || ItemRow->ItemActor.IsNull())
	{
		Finish();
		return;
	}

	OwnerActorRef = OwnerActor;

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(
		ItemRow->ItemActor.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ThisClass::OnClassLoaded, ItemRow->ItemActor)
	);
}

void UGInteractionAction_Spawn::OnClassLoaded(TSoftClassPtr<AGInteractableActor> SoftClass)
{
	UClass* ActorClass = SoftClass.Get();

	if (nullptr == ActorClass || false == OwnerActorRef.IsValid())
	{
		Finish();
		return;
	}

	UWorld* World = OwnerActorRef->GetWorld();
	if (false == IsValid(World))
	{
		Finish();
		return;
	}

	FTransform SpawnTransform = OwnerActorRef->GetActorTransform();
	SpawnTransform.AddToTranslation(FVector(0.f, 0.f, SpawnZOffset));

	World->SpawnActor<AGInteractableActor>(ActorClass, SpawnTransform);

	Finish();
}
