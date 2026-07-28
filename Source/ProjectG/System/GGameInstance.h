#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GGameInstance.generated.h"

class UGEventManager;
class UGDataManager;
class UGUIManagerBase;

UCLASS(Blueprintable)
class PROJECTG_API UGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UGDataManager* GetDataManager() { return DataManager; }
	UGUIManagerBase* GetUIManager() { return UIManager; }
	UGEventManager* GetEventManager() { return EventManager; }
protected:

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<UGDataManager> DataManagerClass;
	
	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<UGUIManagerBase> UIManagerClass;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UGEventManager> EventManager;
	
	UPROPERTY(Transient)
	TObjectPtr<UGDataManager> DataManager;
	
	UPROPERTY(Transient)
	TObjectPtr<UGUIManagerBase> UIManager;
};
