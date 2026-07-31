#pragma once

#include "CoreMinimal.h"
#include "GMessage.generated.h"

UENUM()
enum class EGMessageType
{
	DetectInteractor,
	UndetectInteractor,
	InteractStarted,
	InteractEnded,
};

USTRUCT()
struct FGMessage
{
	GENERATED_BODY()
public:
	FGMessage() { }
	FGMessage(EGMessageType InType) : MessageType(InType) { }

	EGMessageType MessageType;
};

USTRUCT()
struct FGInteract : public FGMessage
{
	GENERATED_BODY()
public:
	FGInteract() { }
	FGInteract(EGMessageType InType) : FGMessage(InType) { }
	FGInteract(EGMessageType InType, FName ID) : FGMessage(InType), ID(ID) { }

	FName ID;
};

USTRUCT()
struct FGInteractHold : public FGMessage
{
	GENERATED_BODY()
public:
	FGInteractHold() { }
	FGInteractHold(EGMessageType InType, float InHoldDuration) : FGMessage(InType), HoldDuration(InHoldDuration) { }

	float HoldDuration = 0.f;
};
