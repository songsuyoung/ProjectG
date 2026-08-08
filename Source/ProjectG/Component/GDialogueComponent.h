#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/GDialogueRow.h"
#include "Interface/GMessageReceiver.h"
#include "GDialogueComponent.generated.h"

class UGDialogueCondition;

USTRUCT(BlueprintType)
struct FGDialogueConditionEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UGDialogueCondition> Condition;

	UPROPERTY(EditAnywhere)
	FName RootDialogueID;
};

UCLASS(ClassGroup=Custom, meta=(BlueprintSpawnableComponent))
class PROJECTG_API UGDialogueComponent : public UActorComponent, public IGMessageReceiver
{
	GENERATED_BODY()

public:
	virtual void OnMessage(FGameplayTag Tag, FGMessage* Message = nullptr) override;

	// InteractionAction_Talk이 바인딩 → 대화 종료 시 Finish() 호출
	FSimpleDelegate OnDialogueEnded;

	void StartDialogue(AActor* TargetActor);
	void StartDialogue(FName RootDialogueID);
	void AdvanceDialogue(FName NextID);
	void AdvanceDialogue();
	void EndDialogue();


protected:
	UPROPERTY(EditAnywhere, Category="Dialogue")
	FName DefaultDialogueID;

	// 우선순위 순서 (위쪽이 먼저 체크)
	UPROPERTY(EditAnywhere, Category="Dialogue")
	TArray<FGDialogueConditionEntry> Conditions;

private:
	FName GetCurrentDialogueID(AActor* TargetActor) const;
	void ShowNode(FName NodeID);
	void SelectChoice(int32 ChoiceIndex);

	FName CurrentNodeID;
	FName CachedNextID;
};
