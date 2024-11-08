#pragma once

#include "CoreMinimal.h"
#include "Weapons/MyWeapon.h"
#include "MyShield.generated.h"

class UBoxComponent;
class USphereComponent;
class AMyCharacter;

UCLASS()
class MANSWAY_API AMyShield : public AMyWeapon
{
	GENERATED_BODY()

public:
	AMyShield();

private:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void SetupComponents();
};
