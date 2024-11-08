#pragma once

#include "CoreMinimal.h"
#include "Weapons/MyWeapon.h"
#include "MySword.generated.h"

UCLASS()
class MANSWAY_API AMySword : public AMyWeapon
{
	GENERATED_BODY()

public:
	AMySword();

private:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void SetupComponents();
	
};
