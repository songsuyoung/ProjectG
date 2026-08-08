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
	EGQuestState State = EGQuestState::Available;
};

UCLASS(ClassGroup=Custom, meta=(BlueprintSpawnableComponent))
class PROJECTG_API UGQuestComponent : public UActorComponent, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;

	// ReadyToComplete 퀘스트 중 이 NPC 것 → 성공 대사 ID
	FName GetSuccessDialogueForNPC(FName NPCID, FName& OutQuestID);
	// Available 퀘스트 중 이 NPC 것 → 제안 대사 ID
	FName GetPreDialogueForNPC(FName NPCID, FName& OutQuestID);

	void AcceptQuest(FName QuestID);
	void FinishObjective(FName QuestID);
	void CompleteQuest(FName QuestID);
	void InitQuests(const TArray<FName>& QuestIDs);

private:
	FGQuestEntry* FindEntry(FName QuestID);

	UPROPERTY()
	TArray<FGQuestEntry> Quests;
};
