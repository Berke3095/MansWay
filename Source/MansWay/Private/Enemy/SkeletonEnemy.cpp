#include "Enemy/SkeletonEnemy.h"

ASkeletonEnemy::ASkeletonEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ASkeletonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
