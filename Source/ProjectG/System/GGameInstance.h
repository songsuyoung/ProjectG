#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GGameInstance.generated.h"

class UGDialogueManager;
class UGEventManager;
class UGDataManager;
class UGUIManagerBase;
class UGConditionManager;
UCLASS(Blueprintable)
class PROJECTG_API UGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UGDataManager* GetDataManager() { return DataManager; }
	UGUIManagerBase* GetUIManager() { return UIManager; }
	UGEventManager* GetEventManager() { return EventManager; }
	UGDialogueManager* GetDialogueManager() { return DialogueManager; }
	UGConditionManager* GetConditionManager() const { return ConditionManager; }
protected:

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<UGDataManager> DataManagerClass;
	
	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<UGUIManagerBase> UIManagerClass;

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<UGConditionManager> ConditionManagerClass;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UGEventManager> EventManager;
	
	UPROPERTY(Transient)
	TObjectPtr<UGDataManager> DataManager;
	
	UPROPERTY(Transient)
	TObjectPtr<UGUIManagerBase> UIManager;
	
	UPROPERTY(Transient)
	TObjectPtr<UGDialogueManager> DialogueManager;

	UPROPERTY(Transient)
	TObjectPtr<UGConditionManager> ConditionManager;
	
};
