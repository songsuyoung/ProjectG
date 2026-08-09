#include "UI/GDialogueWidget.h"

#include "Components/GChoiceButton.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GMessage.h"
#include "Data/GNPCRow.h"
#include "Input/CommonUIInputTypes.h"
#include "System/GDataManager.h"
#include "System/GDialogueManager.h"
#include "System/GEventManager.h"

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
	
}

void UGDialogueWidget::NativeDestruct()
{
	Super::NativeDestruct();
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Dialogue_Node, this);
}

void UGDialogueWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	// 활성화 
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackPressed)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::OnApplyPressed)));
}

void UGDialogueWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	if (ApplyHandle.IsValid())
	{
		ApplyHandle.Unregister();
	}
	if (BackHandle.IsValid())
	{
		BackHandle.Unregister();
	}
}

void UGDialogueWidget::OnBackPressed()
{
	UGDialogueManager* DialogueManager = UGDialogueManager::Get(this);
	
	check(DialogueManager);
	
	DialogueManager->EndDialogue(EGDialogueEndReason::Completed);
}

void UGDialogueWidget::OnApplyPressed()
{
	UGDialogueManager* DialogueManager = UGDialogueManager::Get(this);
	
	check(DialogueManager);
	
	if (false == DialogueManager->NextDialogue())
	{
		if (ApplyHandle.IsValid())
		{
			ApplyHandle.Unregister();
		}
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

}
