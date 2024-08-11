#include "Widgets/PickUpWidget.h"
#include "Components/TextBlock.h" 

void UPickUpWidget::NativeConstruct()
{

}

void UPickUpWidget::SetPickUpText(FString Text1)
{
	if (PickUpText)
	{
		PickUpText->SetText(FText::FromString(Text1));
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UPickUpWidget::SetPickUpText - PickUpText is null.")); }
}
