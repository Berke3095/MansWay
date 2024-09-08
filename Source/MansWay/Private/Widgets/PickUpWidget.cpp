#include "Widgets/PickUpWidget.h"
#include "Components/TextBlock.h" 

void UPickUpWidget::SetPickUpText(const FString& Text1)
{
	if (PickUpText)
	{
		PickUpText->SetText(FText::FromString(Text1));
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UPickUpWidget::SetPickUpText - PickUpText is null.")); }
}
