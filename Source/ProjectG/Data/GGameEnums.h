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
