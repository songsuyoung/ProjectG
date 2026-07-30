#pragma once

#include "CoreMinimal.h"
#include "GMessage.generated.h"

UENUM()
enum class EGMessageType
{
	DetectInteractor,
	UndetectInteractor,
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
	FGInteract(EGMessageType InType, FName ID) : MessageType(InType), ID(ID) { }

	EGMessageType MessageType;
	FName ID;
};
