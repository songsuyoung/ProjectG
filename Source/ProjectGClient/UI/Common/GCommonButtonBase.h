#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "GCommonButtonBase.generated.h"

class UTextBlock;

UCLASS()
class PROJECTGCLIENT_API UGCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	void SetText(const FString& Str);
	void SetText(const FText& Text);
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> TextBlock_Text;
};
