#include "Widgets/MyWidgetManager.h"
#include "Widgets/PickUpWidget.h"
#include "Characters/MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h" 
#include "Weapons/MyShield.h"
#include "Weapons/MySword.h"

AMyWidgetManager* AMyWidgetManager::Instance = nullptr;

AMyWidgetManager::AMyWidgetManager()
{
	PrimaryActorTick.bCanEverTick = true;

	Instance = this;
	SetActorHiddenInGame(true);
}

void AMyWidgetManager::BeginPlay()
{
	Super::BeginPlay();
	
	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(!MyCharacter) { UE_LOG(LogTemp, Error, TEXT("AMyWidgetManager::BeginPlay - MyCharacter is null.")); }
}

void AMyWidgetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WidgetCheck();
}

void AMyWidgetManager::WidgetCheck()
{
	if (PickUpWidget)
	{
		if (MyCharacter)
		{
			MyCharacter->SetClosestInteractable(MyCharacter->CalculateClosestOverlap());
			if (MyCharacter->GetClosestInteractable()->IsA<AMyShield>())
			{
				const FString ShieldText = "Pick Up Shield";
				if (PickUpWidget->GetPickUpTextBlock()->GetText().ToString() != ShieldText)
				{
					PickUpWidget->SetPickUpText(ShieldText);
				}
			}
			else if (MyCharacter->GetClosestInteractable()->IsA<AMySword>())
			{
				const FString SwordText = "Pick Up Sword";
				if (PickUpWidget->GetPickUpTextBlock()->GetText().ToString() != SwordText)
				{
					PickUpWidget->SetPickUpText(SwordText);
				}
			}
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyWidgetManager::WidgetCheck - MyCharacter is null.")); }
	}
}

void AMyWidgetManager::CreatePickUpWidget()
{
	if (PickUpWidgetClass && !PickUpWidget)
	{
		PickUpWidget = CreateWidget<UPickUpWidget>(GetWorld(), PickUpWidgetClass);
		if (PickUpWidget)
		{
			PickUpWidget->AddToViewport();
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyWidgetManager::CreatePickUpWidget - PickUpWidget is null.")); }
	}
	else if(!PickUpWidgetClass) { UE_LOG(LogTemp, Error, TEXT("AMyWidgetManager::CreatePickUpWidget - PickUpWidgetClass is null.")); }
}

void AMyWidgetManager::RemovePickUpWidget()
{
	if (PickUpWidget)
	{
		PickUpWidget->RemoveFromParent();
		PickUpWidget = nullptr;
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWidgetManager::RemovePickUpWidget - PickUpWidget is null.")); }
}
