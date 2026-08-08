#include "UI/GDialogueWidget.h"

#include "Common/GCommonButtonBase.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Data/GDialogueRow.h"
#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GMessage.h"
#include "Data/GNPCRow.h"
#include "Input/CommonUIInputTypes.h"
#include "System/GDataManager.h"
#include "System/GEventManager.h"

void UGDialogueWidget::SelectChoice(int32 Index)
{
	FGDialogueChoiceMessage Msg(Index);
	GEVENT_BROADCAST(this, GGameplayTags::EventTag_Dialogue_SelectChoice, Msg);
}

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
				UGCommonButtonBase* CommonButtonBase = Cast<UGCommonButtonBase>(Child);
				
				if (IsValid(CommonButtonBase))
				{
					CommonButtonBase->OnClicked().AddUObject(this, &ThisClass::OnClick_Choice, ChoiceButtons.Num());
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

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackPressed)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::OnApplyPressed)));
}

void UGDialogueWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (BackHandle.IsValid())
	{
		BackHandle.Unregister();
	}
	
	if (ApplyHandle.IsValid())
	{
		ApplyHandle.Unregister();
	}
}

void UGDialogueWidget::OnBackPressed()
{
	GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Dialogue_Cancel);
}

void UGDialogueWidget::OnApplyPressed()
{
	GEVENT_BROADCAST_EMPTY(this, GGameplayTags::EventTag_Dialogue_Advance);
}

void UGDialogueWidget::OnClick_Choice(int32 Index)
{
	FGDialogueChoiceMessage DialogueChoiceMessage(Index);
	GEVENT_BROADCAST(this, GGameplayTags::EventTag_Dialogue_SelectChoice, DialogueChoiceMessage);
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

	const FGDialogueRow& Row = DialogueMsg->Row;

	if (IsValid(TextBlock_Speaker))
	{
		UGDataManager* DataManager = UGDataManager::Get(this);
		check(DataManager);

		FGNPCRow* NPCRow = DataManager->GetDataTableRow<FGNPCRow>(EGDataTableType::NPC, Row.NPCID);
		if (nullptr != NPCRow)
		{
			TextBlock_Speaker->SetText(NPCRow->Name);
		}
	}

	if (IsValid(TextBlock_Body))
	{
		TextBlock_Body->SetText(Row.Body);
	}

	if (false == Row.Choices.IsEmpty())
	{
		if (IsValid(VBox_Choice))
		{
			VBox_Choice->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		
		// Choices 개수에 따라 visibility 를 킴
		for (int Index = 0; Index < Row.Choices.Num(); Index++)
		{
			if (ChoiceButtons.IsValidIndex(Index))
			{
				ChoiceButtons[Index]->SetText(Row.Choices[Index].ChoiceText);
			}
		}
	}
	else
	{
		if (IsValid(VBox_Choice))
		{
			VBox_Choice->SetVisibility(ESlateVisibility::Hidden);
		}
		
		for (int Index = 0; Index < ChoiceButtons.Num(); Index++)
		{
			ChoiceButtons[Index]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
