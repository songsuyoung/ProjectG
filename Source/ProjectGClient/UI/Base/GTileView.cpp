#include "GTileView.h"
#include "UI/GWidgetFactory.h"

UUserWidget& UGTileView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSubclassOf<UUserWidget> WidgetClass = DesiredEntryClass;

	for (const UGWidgetFactory* Rule : FactoryRules)
	{
		if (IsValid(Rule))
		{
			if (const TSubclassOf<UUserWidget> EntryClass = Rule->FindWidgetClassForData(Item))
			{
				WidgetClass = EntryClass;
				break;
			}
		}
	}

	return GenerateTypedEntry<UUserWidget>(WidgetClass, OwnerTable);
}
