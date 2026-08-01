#pragma once

#include "CoreMinimal.h"
#include "GInteractableActor.h"
#include "GOpenableActor.generated.h"

class UTimelineComponent;

UENUM(BlueprintType)
enum class EGOpenAxis : uint8
{
	Yaw   UMETA(DisplayName = "Yaw"),
	Pitch UMETA(DisplayName = "Pitch"),
	Roll  UMETA(DisplayName = "Roll")
};

UCLASS()
class PROJECTG_API AGOpenableActor : public AGInteractableActor
{
	GENERATED_BODY()

public:
	AGOpenableActor();

	virtual void BeginPlay() override;
	virtual void InternalInteract(AActor* TargetActor) override;

protected:

	UFUNCTION()
	void OnTimelineUpdate(float Value);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Openable|Settings")
	float OpenAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Openable|Settings")
	EGOpenAxis OpenAxis;

	UPROPERTY(EditAnywhere, Category = "Openable")
	TObjectPtr<UCurveFloat> OpenCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline")
	TObjectPtr<UTimelineComponent> TimelineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pivot")
	TObjectPtr<USceneComponent> HingePivotComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> OpeningMeshComponent;
};
