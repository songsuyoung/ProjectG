#include "GInteractionAction_Open.h"

#include "Animation/GAnimInstance.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Gimmick/GInteractableActor.h"
#include "Gimmick/GOpenableActor.h"

UGInteractionAction_Open::UGInteractionAction_Open(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
}

void UGInteractionAction_Open::Execute(AActor* OwnerActor, AActor* TargetActor)
{
	if (false == IsValid(TimelineComponent))
	{
		Finish();
		return;
	}
	
	AGOpenableActor* OpenableActor = Cast<AGOpenableActor>(OwnerActor);
	
	if (false == IsValid(OpenableActor))
	{
		Finish();
		return;
	}
	
	HingePivotComponent = OpenableActor->GetHingePivotComponent();
	
	FOnTimelineFloat TimelineFunction;
	TimelineFunction.BindUFunction(this, FName("OnTimelineUpdate"));
	TimelineComponent->AddInterpFloat(OpenCurve, TimelineFunction);
	TimelineComponent->SetLooping(false);
	
	ACharacter* Character = Cast<ACharacter>(TargetActor);
	
	if (IsValid(Character))
	{
		Character->PlayAnimMontage(AnimMontage);
	}

	if (IsValid(AnimMontage))
	{
		float MontageLength = AnimMontage->GetPlayLength();
		float TimelineLength = TimelineComponent->GetTimelineLength();

		if (MontageLength > 0.f)
		{
			TimelineComponent->SetPlayRate(TimelineLength / MontageLength);
		}
	}
	OpenableMeshComponent = OpenableActor->GetOpenableMeshComponent();
	TargetAnimInstance = Cast<UGAnimInstance>(Character->GetMesh()->GetAnimInstance());
	
	TimelineComponent->Play();
}

void UGInteractionAction_Open::OnTimelineUpdate(float Value)
{
	if (false == HingePivotComponent.IsValid())
	{
		return;
	}

	if (OpenableMeshComponent.IsValid())
	{
		FVector Hand_L= OpenableMeshComponent->GetSocketLocation(TEXT("hand_l"));
		FVector Hand_R= OpenableMeshComponent->GetSocketLocation(TEXT("hand_r"));
	
		if (TargetAnimInstance.IsValid())
		{
			TargetAnimInstance->SetHandIKTarget(Hand_L, Hand_R);
		}
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
	
	if (Value >= 1.f)
	{
		Finish();
		return;
	}
}
