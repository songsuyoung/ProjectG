#include "GInteractionInfoWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Data/GGameMacro.h"
#include "Data/GInteractionPromptRow.h"
#include "Data/GMessage.h"
#include "System/GDataManager.h"
#include "System/GEventManager.h"

void UGInteractionInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GEVENT_MESSAGE_ADD(this, this);
}

void UGInteractionInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();
	GEVENT_MESSAGE_REMOVE(this, this);
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

void UGInteractionInfoWidget::OnMessage(EGMessageType Type, FGMessage* Message)
{
	switch (Type)
	{
	case EGMessageType::DetectInteractor:
		{
			FGInteract* MessageData = static_cast<FGInteract*>(Message);

			if (nullptr != MessageData)
			{
				UpdateUI(MessageData->ID);
			}
		}
		break;
	case EGMessageType::UndetectInteractor:
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	case EGMessageType::InteractStarted:
		{
			FGInteractHold* MessageData = static_cast<FGInteractHold*>(Message);

			if (nullptr != MessageData)
			{
				bIsHolding = true;
				HoldDuration = MessageData->HoldDuration;
				HoldElapsed = 0.f;
			}
		}
		break;
	case EGMessageType::InteractEnded:
		{
			bIsHolding = false;
			HoldElapsed = 0.f;

			if (IsValid(ProgressBar_Hold))
			{
				ProgressBar_Hold->SetPercent(0.f);
			}
		}
		break;
	}
}

void UGInteractionInfoWidget::UpdateUI(FName ID)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	
	// 데이터 테이블에서 ID에 맞는 데이터를 찾고
	UGDataManager* DataManager = UGDataManager::Get(this);
	
	check(DataManager);
	
	FGInteractionPromptRow* PromptRow = DataManager->GetDataTableRow<FGInteractionPromptRow>(EGDataTableType::InteractionPrompt, ID);
	
	if (PromptRow != nullptr)
	{
		// TextBlock_Info값을 변경한다.
		if (IsValid(TextBlock_Info))
		{
			TextBlock_Info->SetText(PromptRow->PromptText);
		}
	}
}
