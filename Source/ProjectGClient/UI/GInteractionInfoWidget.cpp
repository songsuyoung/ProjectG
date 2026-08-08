#include "GInteractionInfoWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GInteractionPromptRow.h"
#include "Data/GMessage.h"
#include "System/GDataManager.h"
#include "System/GEventManager.h"

void UGInteractionInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// Event.Interact 부모 태그로 등록 → 하위 Detect/Undetect/Started/Ended 모두 수신
	GEVENT_ADD(this, GGameplayTags::EventTag_Interact, this);
	GEVENT_ADD(this, GGameplayTags::EventTag_Dialogue_Toggle, this);
}

void UGInteractionInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Interact, this);
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Dialogue_Toggle, this);
}

void UGInteractionInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsHolding && HoldDuration > 0.f)
	{
		HoldElapsed += InDeltaTime;

		if (IsValid(ProgressBar_Hold))
		{
			ProgressBar_Hold->SetPercent(FMath::Clamp(HoldElapsed / HoldDuration, 0.f, 1.f));
		}
	}
}

void UGInteractionInfoWidget::OnMessage(FGameplayTag Tag, FGMessage* Message)
{
	if (Tag == GGameplayTags::EventTag_Dialogue_Toggle)
	{
		bInDialogue = !bInDialogue;
		if (bInDialogue)
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (!LastDetectedID.IsNone())
		{
			UpdateUI(LastDetectedID);
		}
	}
	else if (Tag == GGameplayTags::EventTag_Interact_Detect)
	{
		FGInteract* MessageData = static_cast<FGInteract*>(Message);
		if (nullptr != MessageData)
		{
			UpdateUI(MessageData->ID);
		}
	}
	else if (Tag == GGameplayTags::EventTag_Interact_Undetect)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (Tag == GGameplayTags::EventTag_Interact_Started)
	{
		FGInteractHold* MessageData = static_cast<FGInteractHold*>(Message);
		if (nullptr != MessageData)
		{
			bIsHolding = true;
			HoldDuration = MessageData->HoldDuration;
			HoldElapsed = 0.f;
		}
	}
	else if (Tag == GGameplayTags::EventTag_Interact_Ended)
	{
		bIsHolding = false;
		HoldElapsed = 0.f;

		if (IsValid(ProgressBar_Hold))
		{
			ProgressBar_Hold->SetPercent(0.f);
		}
	}
}

void UGInteractionInfoWidget::UpdateUI(FName ID)
{
	LastDetectedID = ID;
	SetVisibility(ESlateVisibility::HitTestInvisible);

	UGDataManager* DataManager = UGDataManager::Get(this);

	check(DataManager);

	FGInteractionPromptRow* PromptRow = DataManager->GetDataTableRow<FGInteractionPromptRow>(EGDataTableType::InteractionPrompt, ID);

	if (PromptRow != nullptr)
	{
		if (IsValid(TextBlock_Info))
		{
			TextBlock_Info->SetText(PromptRow->PromptText);
		}
	}
}
