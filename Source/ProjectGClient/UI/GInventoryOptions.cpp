#include "GInventoryOptions.h"

#include "CommonButtonBase.h"
#include "Components/VerticalBox.h"

UGInventoryOptions::UGInventoryOptions(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGInventoryOptions::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (false == IsValid(VBox_Option))
	{
		return;
	}
	
	TArray<UWidget*> Childs = VBox_Option->GetAllChildren();
	
	for (int Index = 0; Index < Childs.Num(); Index++)
	{
		UCommonButtonBase* Button = Cast<UCommonButtonBase>(Childs[Index]);
		
		if (IsValid(Button))
		{
			Button->OnClicked().AddUObject(this, &ThisClass::OnClicked, Index);
			Button_Options.Add(Button);
		}
	}
}

void UGInventoryOptions::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGInventoryOptions::OnClicked(int Index)
{
	// Index = 0 사용하기, Index = 1 가방에 보관하기 (가방 -> 단축키 사용 가능)
	EGOptionType OptionType = Index == 0? EGOptionType::Use : EGOptionType::Keep;
	OnClickedOption.ExecuteIfBound(OptionType);
	
	SetVisibility(ESlateVisibility::Collapsed);
}
