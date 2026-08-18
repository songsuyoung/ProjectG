#include "GPrimaryWidget.h"

#include "Components/OverlaySlot.h"
#include "UI/Base/GCommonActivatableWidgetStack.h"
#include "System/GUIWindowLoadMethod.h"

void UGPrimaryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	TArray<UWidget*> ChildWidgets = Root->GetAllChildren();

	for (UWidget* ChildWidget : ChildWidgets)
	{
		UGCommonActivatableWidgetStack* WidgetStack = Cast<UGCommonActivatableWidgetStack>(ChildWidget);

		if (IsValid(WidgetStack))
		{
			LayerMap.Add(WidgetStack->GetLayerType(), WidgetStack);
		}
	}
}

UGCommonActivatableWidget* UGPrimaryWidget::GetOrCreateInstance(const FGameplayTag& WindowTag, const FGameplayTag& LayerTag)
{
	return GetOrCreateInstanceInternal(WindowTag, LayerTag);
}

UGCommonActivatableWidget* UGPrimaryWidget::GetOrCreateInstanceInternal(const FGameplayTag& WindowTag, const FGameplayTag& LayerTag)
{
	TObjectPtr<UCommonActivatableWidgetStack>* WidgetStack = LayerMap.Find(LayerTag);

	if (nullptr == WidgetStack)
	{
		return nullptr;
	}
	UGCommonActivatableWidget* ReturnWidget = nullptr;
	UGUIWindowLoadMethod* WindowLoadMethodCDO = WindowLoadMethodClass->GetDefaultObject<UGUIWindowLoadMethod>();

	if (false == IsValid(WindowLoadMethodCDO))
	{
		return nullptr;
	}

	TSubclassOf<UGCommonActivatableWidget> ActivatableWidgetClass = WindowLoadMethodCDO->LoadAssetClass(WindowTag);

	if (false == IsValid(ActivatableWidgetClass))
	{
		return nullptr;
	}

	FGPoolContainer* Pool = PoolingContainer.Find(ActivatableWidgetClass->GetFName());

	if (nullptr != Pool)
	{
		int32 LastIndex = Pool->Container.Num() - 1;

		if (LastIndex >= 0)
		{
			ReturnWidget = Pool->Container[LastIndex];
			Pool->Container.RemoveSingleSwap(ReturnWidget);
			(*WidgetStack)->AddWidgetInstance(*ReturnWidget);
		}
	}

	if (false == IsValid(ReturnWidget))
	{
		ReturnWidget = Cast<UGCommonActivatableWidget>((*WidgetStack)->AddWidget(ActivatableWidgetClass));
	}
	
	if (IsValid(ReturnWidget))
	{
		ReturnWidget->Init(this);
	}
	
	return ReturnWidget;
}

void UGPrimaryWidget::ReturnToPool(UGCommonActivatableWidget* Widget)
{
	if (false == IsValid(Widget))
	{
		return;
	}
	
	for (auto& Pair : LayerMap)
	{
		if (IsValid(Pair.Value) && Pair.Value->GetActiveWidget() == Widget)
		{
			Pair.Value->RemoveWidget(*Widget);
			break;
		}
	}

	if (Widget->IsPoolable())
	{
		FGPoolContainer& Pool = PoolingContainer.FindOrAdd(Widget->GetClass()->GetFName());
		Pool.Container.Add(Widget);
	}
}
