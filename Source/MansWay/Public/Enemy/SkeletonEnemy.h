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
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void AttachStartingWeapon();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AMyWeapon> WeaponClass{};

	void Attack() override;

	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) override;
};
