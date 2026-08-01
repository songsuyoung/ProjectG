#include "Gimmick/GOpenableActor.h"

#include "Components/TimelineComponent.h"

AGOpenableActor::AGOpenableActor()
	: Super()
	, OpenAngle(110.f)
	, OpenAxis(EGOpenAxis::Yaw)
{
	HingePivotComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HingePivotComponent"));
	HingePivotComponent->SetupAttachment(MeshComponent);

	OpeningMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpeningMeshComponent"));
	OpeningMeshComponent->SetupAttachment(HingePivotComponent);

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
}

void AGOpenableActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(TimelineComponent))
	{
		FOnTimelineFloat TimelineFunction;
		TimelineFunction.BindUFunction(this, FName("OnTimelineUpdate"));
		TimelineComponent->AddInterpFloat(OpenCurve, TimelineFunction);
		TimelineComponent->SetLooping(false);
	}
}

void AGOpenableActor::InternalInteract(AActor* TargetActor)
{
	Super::InternalInteract(TargetActor);

	if (IsValid(TimelineComponent))
	{
		TimelineComponent->Play();
	}
}

void AGOpenableActor::OnTimelineUpdate(float Value)
{
	if (!IsValid(HingePivotComponent))
	{
		return;
	}

	FRotator Rotation = FRotator::ZeroRotator;
	switch (OpenAxis)
	{
	case EGOpenAxis::Yaw:
		Rotation.Yaw = OpenAngle * Value;
		break;
	case EGOpenAxis::Pitch:
		Rotation.Pitch = OpenAngle * Value;
		break;
	case EGOpenAxis::Roll:
		Rotation.Roll = OpenAngle * Value;
		break;
	}

	HingePivotComponent->SetRelativeRotation(Rotation);
}
