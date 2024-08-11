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

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PickUpText{};

public:
	void SetPickUpText(FString Text1);
	FORCEINLINE const UTextBlock* GetPickUpTextBlock() const { return PickUpText ? PickUpText : nullptr; }
};
