#pragma once

#include "CoreMinimal.h"
#include "GUIWindowLoadMethodBase.generated.h"

class UClass;

UCLASS()
class PROJECTG_API UGUIWindowLoadMethodBase : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
	UClass* LoadAssetClass(UClass* Class);
	
	virtual UClass* LoadAssetClass_Implementation(UClass* Class) { return nullptr; }
};
