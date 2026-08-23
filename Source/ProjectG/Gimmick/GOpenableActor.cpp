#include "Gimmick/GOpenableActor.h"

#include "Components/TimelineComponent.h"

AGOpenableActor::AGOpenableActor()
	: Super()
{
	HingePivotComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HingePivotComponent"));
	HingePivotComponent->SetupAttachment(MeshComponent);

	OpeningMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpeningMeshComponent"));
	OpeningMeshComponent->SetupAttachment(HingePivotComponent);
}
