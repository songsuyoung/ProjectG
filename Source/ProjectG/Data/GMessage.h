#pragma once

#include "CoreMinimal.h"
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
struct FGItemMessage : public FGMessage
{
	GENERATED_BODY()
public:
	FGItemMessage() { }
	FGItemMessage(FName InItemID, int32 InItemCount) : ItemID(InItemID), ItemCount(InItemCount) { }

	FName ItemID;
	int32 ItemCount;
};
