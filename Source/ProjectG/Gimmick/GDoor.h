#pragma once

#include "CoreMinimal.h"
#include "GInteractableActor.h"
#include "GameFramework/Actor.h"
#include "GDoor.generated.h"

class UTimelineComponent;
UCLASS()
class PROJECTG_API AGDoor : public AGInteractableActor
{
	GENERATED_BODY()
	
public:	
	AGDoor();
	
	virtual void BeginPlay() override;
	virtual void InternalInteract(AActor* TargetActor) override;
	
protected:
	
	UFUNCTION()
	void OpenDoor(float Value);
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Settings")
	float OpenAngle;
	
	UPROPERTY(EditAnywhere, Category = "Door")
	TObjectPtr<UCurveFloat> DoorCurve;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline)
	TObjectPtr<UTimelineComponent> TimelineComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pivot")
	TObjectPtr<USceneComponent> HingePivotComponent;	// 경첩을 의미 (접합부)
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> DoorMeshComponent;
};
