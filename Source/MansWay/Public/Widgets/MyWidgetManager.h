#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWidgetManager.generated.h"

class UPickUpWidget;
class AMyCharacter;

UCLASS()
class MANSWAY_API AMyWidgetManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyWidgetManager();

private:
	static AMyWidgetManager* Instance;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void WidgetCheck();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPickUpWidget> PickUpWidgetClass{};
	UPickUpWidget* PickUpWidget{};

	AMyCharacter* MyCharacter{};

	const FString ShieldText = "Pick Up Shield";
	const FString SwordText = "Pick Up Sword";

public:
	static AMyWidgetManager* GetInstance() { return Instance ? Instance : nullptr; }
	void CreatePickUpWidget();
	void RemovePickUpWidget();

};
