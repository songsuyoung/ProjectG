#include "GChoiceButton.h"

#include "Data/GDialogueRow.h"

void UGChoiceButton::Init(const FGDialogueChoice& Choice)
{
  	SetVisibility(ESlateVisibility::Visible);
	Super::SetText(Choice.ChoiceText);
	DialogueChoice = Choice;
}
