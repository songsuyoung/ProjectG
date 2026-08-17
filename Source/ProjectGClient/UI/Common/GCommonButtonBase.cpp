#include "UI/Common/GCommonButtonBase.h"

#include "Components/TextBlock.h"

void UGCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (IsDesignTime())
	{
		SetText(Label);
	}
}

void UGCommonButtonBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetText(Label);
}

void UGCommonButtonBase::SetText(const FString& Str)
{
	FText Text = FText::FromString(Str);
	SetText(Text);
}

void UGCommonButtonBase::SetText(const FText& Text)
{
	if (IsValid(TextBlock_Text))
	{
		TextBlock_Text->SetText(Text);
	}
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
