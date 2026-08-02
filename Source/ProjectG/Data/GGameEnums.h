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
	Max,
};

ENUM_RANGE_BY_COUNT(EGDataTableType, EGDataTableType::Max);