#pragma once

#include "CoreMinimal.h"
#include "Weapons/MyWeapon.h"
#include "MyAxe.generated.h"

UCLASS()
class MANSWAY_API AMyAxe : public AMyWeapon
{
	GENERATED_BODY()
	
public:
	AMyAxe();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetupComponents();
};
