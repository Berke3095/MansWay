#include "Enemy/SkeletonEnemy.h"
#include "Controllers/MyAIController.h"
#include "Characters/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/EnemyCombatComponent.h"
#include "Weapons/MyWeapon.h"

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

	AttachStartingWeapon();
}

void ASkeletonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyCharacter && MyAIController)
	{
		MyAIController->MoveToActor(MyCharacter, 50.0f);
	}
}

void ASkeletonEnemy::AttachStartingWeapon()
{
	const USkeletalMeshSocket* weaponSocket = MeshComponent->GetSocketByName(FName("Sword_Socket"));
	FTransform weaponSocketTransform = weaponSocket->GetSocketTransform(MeshComponent);
	if (WeaponClass)
	{
		AActor* weapon = GetWorld()->SpawnActor<AActor>(WeaponClass, weaponSocketTransform);
		if (weapon && CombatComponent)
		{
			CombatComponent->EquipInteractable(this, weapon);
		}
		else { UE_LOG(LogTemp, Error, TEXT("ASkeletonEnemy::AttachStartingWeapon - Weapon is null.")) }
	}
	else {UE_LOG(LogTemp, Error, TEXT("ASkeletonEnemy::AttachStartingWeapon - WeaponClass is null.")) }
}
