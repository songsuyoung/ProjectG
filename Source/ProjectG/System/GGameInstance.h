#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GGameInstance.generated.h"

class UGDialogueManager;
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
	UGDialogueManager* GetDialogueManager() { return DialogueManager; }
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
	
	UPROPERTY(Transient)
	TObjectPtr<UGDialogueManager> DialogueManager;
};
