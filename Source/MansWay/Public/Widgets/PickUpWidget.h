#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickUpWidget.generated.h"

class UTextBlock;

UCLASS()
class MANSWAY_API UPickUpWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PickUpText{};

public:
	void SetPickUpText(const FString &text);
	FORCEINLINE const UTextBlock* GetPickUpTextBlock() const { return PickUpText ? PickUpText : nullptr; }
};
