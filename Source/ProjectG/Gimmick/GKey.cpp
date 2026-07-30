#include "Gimmick/GKey.h"

#include "ContentBrowserDataSource.h"
#include "Character/GCharacter.h"
#include "Component/GInventoryComponent.h"

AGKey::AGKey()
	: Super()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
}

void AGKey::InternalInteract(AActor* TargetActor)
{
	Super::InternalInteract(TargetActor);
	
	AGCharacter* Character = Cast<AGCharacter>(TargetActor);
	
	if (IsValid(Character))
	{
		UGInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
		
		if (IsValid(InventoryComponent))
		{
			InventoryComponent->Acquire(ID);
		}
	}
	
	SetLifeSpan(3.0f);
}
