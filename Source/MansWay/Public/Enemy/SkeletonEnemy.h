#pragma once

#include "CoreMinimal.h"
#include "Enemy/MyEnemy.h"
#include "SkeletonEnemy.generated.h"

class AMyWeapon;

UCLASS()
class MANSWAY_API ASkeletonEnemy : public AMyEnemy
{
	GENERATED_BODY()

public:
	ASkeletonEnemy();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void AttachStartingWeapon();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AMyWeapon> WeaponClass{};
};
