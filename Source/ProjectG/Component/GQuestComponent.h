#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/GGameEnums.h"
#include "Interface/GMessageReceiver.h"
#include "GQuestComponent.generated.h"

USTRUCT(BlueprintType)
struct FGQuestEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGQuestState State = EGQuestState::None;
};

USTRUCT()
struct FGDialogueEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	FName QuestID;
	
	UPROPERTY(Transient)
	EGQuestState State = EGQuestState::None;
		
	UPROPERTY(Transient)
	FName DialogueID;
};

UCLASS(ClassGroup=Custom, meta=(BlueprintSpawnableComponent))
class PROJECTG_API UGQuestComponent : public UActorComponent, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	FName GetDialogueForNPC(FName NPCID);
	EGQuestState GetQuestState(FName QuestID) const;
	bool CanAcceptQuest(FName QuestID);

	void OnNPCDialogueCompleted(FName NPCID);
	void CheckQuest(const FName& NPCID);
	bool AcceptQuest(FName QuestID);
	void FinishObjective(FName QuestID);
protected:
	void InitQuest();
	bool CompleteQuest(FName QuestID);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;
protected:
	FGQuestEntry* FindEntry(FName QuestID);
	const FGQuestEntry* FindEntry(FName QuestID) const;

protected:
	UPROPERTY(Transient)
	TArray<FGQuestEntry> Quests;

	UPROPERTY(Transient)
	TArray<FName> ActiveQuestIDs;
	
	UPROPERTY(Transient)
	FName PendingQID;
};
