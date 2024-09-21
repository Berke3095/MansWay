#include "Enemy/SkeletonEnemy.h"
#include "Controllers/MyAIController.h"
#include "Characters/MyCharacter.h"

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

	/*if (MyCharacter && MyAIController)
	{
		MyAIController->MoveToActor(MyCharacter, 50.0f);
	}*/
}
