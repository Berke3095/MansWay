#include "Enemy/SkeletonEnemy.h"
#include "Controllers/MyAIController.h"
#include "Characters/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ASkeletonEnemy::ASkeletonEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	YawLimit = 140.f;
	PitchLimit = 90.f;

	DefaultSpeed = 250.0f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
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

	UE_LOG(LogTemp, Warning, TEXT("Yaw: %f, Pitch: %f"), GetEnemyYaw(), GetEnemyPitch());
}
