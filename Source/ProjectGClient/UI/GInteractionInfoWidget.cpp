#include "GInteractionInfoWidget.h"

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

void UGInteractionInfoWidget::OnMessage(EGMessageType Type, FGMessage* Message)
{
	switch (Type)
	{
	case EGMessageType::UpdateInterator:
		{
			FGInteract* MessageData = static_cast<FGInteract*>(Message);
	
			if (nullptr != MessageData)
			{
				UpdateUI(MessageData->ID);
			}
		}
		break;
	}
}

void UGInteractionInfoWidget::UpdateUI(FName ID)
{
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
