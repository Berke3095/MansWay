#include "Widgets/PickUpWidget.h"
#include "Components/TextBlock.h" 

void UPickUpWidget::SetPickUpText(const FString& text)
{
	if (PickUpText)
	{
		PickUpText->SetText(FText::FromString(text));
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UPickUpWidget::SetPickUpText - PickUpText is null.")); }
}
