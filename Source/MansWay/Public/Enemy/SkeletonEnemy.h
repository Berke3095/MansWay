#pragma once

#include "CoreMinimal.h"
#include "Enemy/MyEnemy.h"
#include "SkeletonEnemy.generated.h"

UCLASS()
class MANSWAY_API ASkeletonEnemy : public AMyEnemy
{
	GENERATED_BODY()

public:
	ASkeletonEnemy();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
