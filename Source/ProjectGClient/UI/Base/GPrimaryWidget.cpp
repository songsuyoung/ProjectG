#include "GPrimaryWidget.h"

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

UGCommonActivatableWidget* UGPrimaryWidget::GetOrCreateInstance(UClass* Class, const FGameplayTag& LayerTag)
{
	return GetOrCreateInstanceInternal(Class, LayerTag);
}

UGCommonActivatableWidget* UGPrimaryWidget::GetOrCreateInstanceInternal(UClass* Class, const FGameplayTag& LayerTag)
{
	TObjectPtr<UCommonActivatableWidgetStack>* WidgetStack = LayerMap.Find(LayerTag);

	if (nullptr == WidgetStack)
	{
		return nullptr;
	}
	UGCommonActivatableWidget* ReturnWidget = nullptr;

	FGPoolContainer* Pool = PoolingContainer.Find(Class->GetFName());

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

	if (ReturnWidget == nullptr)
	{
		UGUIWindowLoadMethod* WindowLoadMethodCDO = WindowLoadMethodClass->GetDefaultObject<UGUIWindowLoadMethod>();

		if (false == IsValid(WindowLoadMethodCDO))
		{
			return nullptr;
		}

		TSubclassOf<UGCommonActivatableWidget> ActivatableWidgetClass = WindowLoadMethodCDO->LoadAssetClass(Class);

		if (false == IsValid(ActivatableWidgetClass))
		{
			return nullptr;
		}

		ReturnWidget = Cast<UGCommonActivatableWidget>((*WidgetStack)->AddWidget(ActivatableWidgetClass));
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