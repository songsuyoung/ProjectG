#pragma once


UENUM(BlueprintType)
enum class EGInteractionState : uint8
{
	None,
	Available,
	Unavailable,
	Pending,
	Max
};

UENUM(BlueprintType)
enum class EGDataTableType : uint8
{
	InteractionPrompt,
	Item,
	Dialogue,
	Quest,
	NPC,
	Condition,
	Workbench,
	Max,
};

ENUM_RANGE_BY_COUNT(EGDataTableType, EGDataTableType::Max);

UENUM(BlueprintType)
enum class EGQuestState : uint8
{
	None,
	Available,
	Active,
	ReadyToComplete,
	Completed,
};

UENUM(BlueprintType)
enum class EGDialogueEndReason : uint8
{
	Completed,
	Cancelled,
	Invalid,
};


UENUM(BlueprintType)
enum class EGConditionType : uint8
{
	LevelComp,
	ItemComp,
	MapVisited,
	QuestState,
	KillCount,
};
