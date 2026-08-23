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
	USceneComponent* GetHingePivotComponent() { return HingePivotComponent; }
	UMeshComponent* GetOpenableMeshComponent() { return OpeningMeshComponent; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pivot")
	TObjectPtr<USceneComponent> HingePivotComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> OpeningMeshComponent;
};
