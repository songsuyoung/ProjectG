#pragma once

#include "CoreMinimal.h"
#include "GInteractionAction.h"
#include "GInteractionAction_Open.generated.h"

class UTimelineComponent;
class USceneComponent;
class UGAnimInstance;
enum class EGOpenAxis : uint8;

UCLASS()
class PROJECTG_API UGInteractionAction_Open : public UGInteractionAction
{
	GENERATED_BODY()

public:
	UGInteractionAction_Open(const FObjectInitializer& ObjectInitializer);
	virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
	
protected:
	
	UFUNCTION()
	void OnTimelineUpdate(float Value);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Openable|Settings")
	TObjectPtr<UAnimMontage> AnimMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Openable|Settings")
	float OpenAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Openable|Settings")
	EGOpenAxis OpenAxis;

	UPROPERTY(EditAnywhere, Category = "Openable")
	TObjectPtr<UCurveFloat> OpenCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timeline")
	TObjectPtr<UTimelineComponent> TimelineComponent;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Pivot")
	TWeakObjectPtr<USceneComponent> HingePivotComponent;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Pivot")
	TWeakObjectPtr<UMeshComponent> OpenableMeshComponent;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Pivot")
	TWeakObjectPtr<UGAnimInstance> TargetAnimInstance;
	
};
