#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GUIWindowLoadMethodBase.generated.h"

class UClass;

UCLASS()
class PROJECTG_API UGUIWindowLoadMethodBase : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
	UClass* LoadAssetClass(FGameplayTag WindowTag);
	
	virtual UClass* LoadAssetClass_Implementation(FGameplayTag WindowTag) { return nullptr; }
};
