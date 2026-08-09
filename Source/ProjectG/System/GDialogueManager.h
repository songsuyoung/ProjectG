#pragma once

#include "CoreMinimal.h"
#include "Interface/GMessageReceiver.h"
#include "UObject/NoExportTypes.h"
#include "GDialogueManager.generated.h"

struct FGDialogueChoice;
enum class EGDialogueEndReason : uint8;
DECLARE_MULTICAST_DELEGATE_OneParam(FGOnDialogueEnded, EGDialogueEndReason);
UCLASS()
class PROJECTG_API UGDialogueManager : public UObject
{
	GENERATED_BODY()
	
public:
	static UGDialogueManager* Get(const UObject* Context);
	void StartDialogue(const FName& DID);
	bool NextDialogue();
	void EndDialogue(EGDialogueEndReason EndReason);
	void SelectChoice(const FGDialogueChoice& Choice);
	
	FGOnDialogueEnded OnDialogueEnded;
protected:

	void ShowNode(FName NodeID);
protected:
	
	UPROPERTY(Transient)
	FName CurrentNodeID;
	
	UPROPERTY(Transient)
	FName CachedNextID;
};
