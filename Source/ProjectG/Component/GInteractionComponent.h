#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GInteractionComponent.generated.h"

USTRUCT()
struct FGInteractionCandidate
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> Actor;
	
	UPROPERTY(Transient)
	int32 Priority;
	
	UPROPERTY(Transient)
	float Score;
};

class ACharacter;
class USphereComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTG_API UGInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGInteractionComponent(const FObjectInitializer& ObjectInitializer);

	void OnInteractStarted();
	void Interact();

protected:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	void UpdateFocusTarget();
	void UpdateCandidateScores();
	float CalculateScore(const TWeakObjectPtr<AActor>& Candidate);
	static bool CompareCandidates(const FGInteractionCandidate &ACandidate, const FGInteractionCandidate &BCandidate);
protected:
	
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	
	// 인터랙션 시 사용할 내적
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction)
	float ViewDotThreshold;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> InteractionSphereComponent;
	
protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<ACharacter> CharacterRef;
	
	UPROPERTY(Transient)
	TArray<FGInteractionCandidate> InteractableCandidates;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> InteractableActor;
};
