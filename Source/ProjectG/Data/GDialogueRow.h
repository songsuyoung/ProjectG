#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GDialogueRow.generated.h"

USTRUCT(BlueprintType)
struct FGDialogueChoice
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ChoiceText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextID;

	// 설정 시 이 선택지 선택 → AcceptQuest(QuestID) 호출
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName QuestID;
};

USTRUCT(BlueprintType)
struct FGDialogueRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems);
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName);

	FName GetID() const { return ID; }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NPCID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Body;

	// 선택지 없을 때 다음 노드
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextID;

	// 선택지 있으면 NextID 무시
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGDialogueChoice> Choices;

private:
	FName ID;
};
