#include "UI/GDialogueWidget.h"

#include "Components/GChoiceButton.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "CommonActionWidget.h"

#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GMessage.h"
#include "Data/GNPCRow.h"
#include "System/GDataManager.h"
#include "System/GDialogueManager.h"
#include "System/GEventManager.h"
#include "System/GUIManagerBase.h"

void UGDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GEVENT_ADD(this, GGameplayTags::EventTag_Dialogue_Node, this);
	
	for (UWidget* Widget :VBox_Choice->GetAllChildren())
	{
		UHorizontalBox* HBox = Cast<UHorizontalBox>(Widget);
		
		if (IsValid(HBox))
		{
			for (UWidget* Child : HBox->GetAllChildren())
			{
				UGChoiceButton* CommonButtonBase = Cast<UGChoiceButton>(Child);
				
				if (IsValid(CommonButtonBase))
				{
					CommonButtonBase->OnClicked().AddUObject(this, &ThisClass::OnChoiceClicked, ChoiceButtons.Num());
					ChoiceButtons.Add(CommonButtonBase);
				}
			}
		}
	}
	
	if (IsValid(Button_Next))
	{
		Button_Next->OnClicked().AddUObject(this, &ThisClass::OnApplyPressed);
	}
	
	if (IsValid(Button_Exit))
	{
		Button_Exit->OnClicked().AddUObject(this, &ThisClass::OnBackPressed);
	}
}

void UGDialogueWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	for (UGChoiceButton* CommonButtonBase : ChoiceButtons)
	{
		if (IsValid(CommonButtonBase))
		{
			CommonButtonBase->OnClicked().RemoveAll(this);
		}
	}
	
	ChoiceButtons.Empty();
	
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Dialogue_Node, this);
	
	if (IsValid(Button_Next))
	{
		Button_Next->OnClicked().RemoveAll(this);
	}
	
	if (IsValid(Button_Exit))
	{
		Button_Exit->OnClicked().RemoveAll(this);
	}
}

void UGDialogueWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	if (IsValid(Button_Next))
	{
		Button_Next->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(Button_Exit))
	{
		Button_Exit->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGDialogueWidget::OnBackPressed()
{
	UGDialogueManager* DialogueManager = UGDialogueManager::Get(this);
	
	check(DialogueManager);
	
	DialogueManager->EndDialogue(EGDialogueEndReason::Completed);
	
	UGUIManagerBase* UIManager = UGUIManagerBase::Get(this);

	check(UIManager);

	UIManager->CloseWindow(this);
}

void UGDialogueWidget::OnApplyPressed()
{
	UGDialogueManager* DialogueManager = UGDialogueManager::Get(this);
	
	check(DialogueManager);
	
	if (false == DialogueManager->NextDialogue())
	{
		UpdateNextExitButtons(false);
	}
}

void UGDialogueWidget::OnChoiceClicked(int32 Index)
{
	if (false == ChoiceButtons.IsValidIndex(Index))
	{
		return;
	}
	
	UGDialogueManager* DialogueManager = UGDialogueManager::Get(this);
	
	check(DialogueManager);
	
	DialogueManager->SelectChoice(ChoiceButtons[Index]->GetData());
}

void UGDialogueWidget::UpdateNextExitButtons(bool bShowNext)
{
	if (IsValid(Button_Next))
	{
		Button_Next->SetVisibility(bShowNext ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (IsValid(Button_Exit))
	{
		Button_Exit->SetVisibility(bShowNext ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}

void UGDialogueWidget::OnMessage(FGameplayTag Tag, FGMessage* Message)
{
	if (Tag != GGameplayTags::EventTag_Dialogue_Node)
	{
		return;
	}

	FGDialogueMessage* DialogueMsg = static_cast<FGDialogueMessage*>(Message);
	if (nullptr == DialogueMsg)
	{
		return;
	}
	
	if (IsValid(TextBlock_Speaker))
	{
		UGDataManager* DataManager = UGDataManager::Get(this);
		check(DataManager);

		FGNPCRow* NPCRow = DataManager->GetDataTableRow<FGNPCRow>(EGDataTableType::NPC, DialogueMsg->NPCID);
		if (nullptr != NPCRow)
		{
			TextBlock_Speaker->SetText(NPCRow->Name);
		}
	}

	if (IsValid(TextBlock_Body))
	{
		TextBlock_Body->SetText(DialogueMsg->Body);
	}
	
	for (int Index = 0; Index < ChoiceButtons.Num(); Index++)
	{
		ChoiceButtons[Index]->SetVisibility(ESlateVisibility::Hidden);
	}
	
	for (int Index = 0; Index < DialogueMsg->Choices.Num(); Index++)
	{
		ChoiceButtons[Index]->Init(DialogueMsg->Choices[Index]);
	}

	if (DialogueMsg->Choices.IsEmpty())
	{
		UpdateNextExitButtons(DialogueMsg->bIsNext);
	}
}
