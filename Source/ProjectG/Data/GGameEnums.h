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