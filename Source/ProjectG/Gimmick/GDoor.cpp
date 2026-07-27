#include "Gimmick/GDoor.h"

#include "Components/TimelineComponent.h"

AGDoor::AGDoor()
	: Super()
	, OpenAngle(110.f)
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameStaticMeshComponent"));
	
	HingePivotComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HingPivotComponent"));
	HingePivotComponent->SetupAttachment(MeshComponent);
	
	DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshComponent"));
	DoorMeshComponent->SetupAttachment(HingePivotComponent);
	
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
}

void AGDoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(TimelineComponent))
	{
		FOnTimelineFloat TimelineFunction;
		TimelineFunction.BindUFunction(this, FName("OpenDoor"));
		TimelineComponent->AddInterpFloat(DoorCurve,TimelineFunction);
		TimelineComponent->SetLooping(false);
	}
}

void AGDoor::InternalInteract(AActor* TargetActor)
{
	// 문이 열리는 애니메이션이 동작한다.
	if (IsValid(TimelineComponent))
	{
		TimelineComponent->Play();
	}
}

void AGDoor::OpenDoor(float Value)
{
	if (IsValid(HingePivotComponent))
	{
		HingePivotComponent->SetRelativeRotation(FRotator(0.f, OpenAngle * Value, 0.f));
	}
}

