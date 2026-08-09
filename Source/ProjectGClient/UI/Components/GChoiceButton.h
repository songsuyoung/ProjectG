#pragma once

#include "CoreMinimal.h"
#include "Data/GDialogueRow.h"
#include "UI/Common/GCommonButtonBase.h"
#include "GChoiceButton.generated.h"

class UTextBlock;
struct FGDialogueChoice;
UCLASS()
class PROJECTGCLIENT_API UGChoiceButton : public UGCommonButtonBase
{
	GENERATED_BODY()
	
public:
	const FGDialogueChoice& GetData() { return DialogueChoice;}
	void Init(const FGDialogueChoice& Choice);
protected:
	
	UPROPERTY(Transient)
	FGDialogueChoice DialogueChoice;
};
