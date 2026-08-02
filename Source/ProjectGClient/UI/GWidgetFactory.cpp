#include "UI/GWidgetFactory.h"

TSubclassOf<UUserWidget> UGWidgetFactory::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	return TSubclassOf<UUserWidget>();
}

TSubclassOf<UUserWidget> UGWidgetFactory_Class::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	for (UClass* Class = Data->GetClass(); Class; Class = Class->GetSuperClass())
	{
		TSoftClassPtr<UObject> ClassPtr(Class);
		if (const TSubclassOf<UUserWidget>* Found = EntryWidgetForClass.Find(ClassPtr))
		{
			return *Found;
		}
	}

	return TSubclassOf<UUserWidget>();
}
