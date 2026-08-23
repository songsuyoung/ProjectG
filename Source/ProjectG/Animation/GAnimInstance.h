#pragma once

#include "CoreMinimal.h"
#include "GBaseAnimInstance.h"
#include "GAnimInstance.generated.h"

UCLASS()
class PROJECTG_API UGAnimInstance : public UGBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	
	void SetHandIKTarget(const FVector& Left, const FVector& Right) { HandIKTarget_L = Left; HandIKTarget_R = Right; }
	
protected:
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	FVector HandIKTarget_R;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
	FVector HandIKTarget_L;
};
