#include "Gimmick/GInteractableActor.h"

AGInteractableActor::AGInteractableActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComponent;
}

void AGInteractableActor::Interact(AActor* TargetActor)
{
	// 접촉 후 3초 후에 사라진다.
	SetLifeSpan(3.0f);
}
