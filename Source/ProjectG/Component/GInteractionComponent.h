#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GInteractionComponent.generated.h"

class ACharacter;
class USphereComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTG_API UGInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGInteractionComponent();

	void Interact();

protected:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	void UpdateFocusTarget();
	float CalculateScore(const TWeakObjectPtr<AActor>& Candidate);
	
protected:
	
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	
	// 인터랙션 시 사용할 내적
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction)
	float ViewDotThreshold;
	
	// 인터랙션 타겟으로 정할 개수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction)
	int32 MaxInteractableCandidates;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> InteractionSphereComponent;
	
protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<ACharacter> CharacterRef;
	
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> InteractableCandidates;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> InteractableActor;
};
