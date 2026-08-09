#pragma once

#include "CoreMinimal.h"
#include "Data/GDialogueRow.h"
#include "GMessage.generated.h"

USTRUCT()
struct FGMessage
{
	GENERATED_BODY()

public:
	FGMessage() { }
};

USTRUCT()
struct FGInteract : public FGMessage
{
	GENERATED_BODY()

public:
	FGInteract() { }
	FGInteract(FName InID) : ID(InID) { }

	FName ID;
};

USTRUCT()
struct FGInteractHold : public FGMessage
{
	GENERATED_BODY()

public:
	FGInteractHold() { }
	FGInteractHold(float InHoldDuration) : HoldDuration(InHoldDuration) { }

	float HoldDuration = 0.f;
};

USTRUCT()
struct FGDialogueMessage : public FGMessage
{
	GENERATED_BODY()

public:
	FGDialogueMessage() { }
	FGDialogueMessage(const FName& NPCID, const FText& Body, const TArray<FGDialogueChoice>& Choices) : NPCID(NPCID), Body(Body), Choices(Choices) { }

	FName NPCID;
	FText Body;
	
	UPROPERTY(Transient)
	TArray<FGDialogueChoice> Choices;
};

USTRUCT()
struct FGQuestMessage : public FGMessage
{
	GENERATED_BODY()

public:
	FGQuestMessage() {}
	FGQuestMessage(FName InQuestID) : QuestID(InQuestID) {}

	FName QuestID;
};

USTRUCT()
struct FGItemMessage : public FGMessage
{
	GENERATED_BODY()

public:
	FGItemMessage() { }
	FGItemMessage(FName InItemID, int32 InItemCount) : ItemID(InItemID), ItemCount(InItemCount) { }

	FName ItemID;
	int32 ItemCount;
};
