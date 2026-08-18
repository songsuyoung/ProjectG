#pragma once

#include "CoreMinimal.h"
#include "UI/GUIWindowLoadMethodBase.h"
#include "UI/Base/GCommonActivatableWidget.h"
#include "GUIWindowLoadMethod.generated.h"

UCLASS(Blueprintable)
class PROJECTGCLIENT_API UGUIWindowLoadMethod : public UGUIWindowLoadMethodBase
{
	GENERATED_BODY()
	
public:
	
	virtual UClass* LoadAssetClass_Implementation(FGameplayTag WindowTag) override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Window | UI")
	TMap<FGameplayTag, TSoftClassPtr<UGCommonActivatableWidget>> WindowMap;
};
