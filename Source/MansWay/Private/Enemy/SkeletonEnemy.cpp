#include "Enemy/SkeletonEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/EnemyCombatComponent.h"
#include "Weapons/MyWeapon.h"
#include "Animations/EnemyAnimInstance.h"

ASkeletonEnemy::ASkeletonEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	YawLimit = 140.f;
	PitchLimit = 90.f;

	DefaultSpeed = 200.0f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

	AttackRange = 250.0f;
	Avoidance = 50.0f;
	AttackCooldown = 1.25f;
}

void ASkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();

	AttachStartingWeapon();
}

void ASkeletonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void ASkeletonEnemy::Attack()
{
	if (AnimInstance && EnemyCombatState == EEnemyCombatState::EECS_Attacking)
	{
		if (AttackMontage)
		{
			if (!AnimInstance->Montage_IsPlaying(AttackMontage))
			{
				FName sectionName{};
				int32 randomNum = FMath::RandRange(1, 2);

				switch (randomNum)
				{
				case 1:
					sectionName = "Attack_1";
					break;
				case 2:
					sectionName = "Attack_2";
					break;
				default:
					break;
				}

				AnimInstance->Montage_Play(AttackMontage);
				AnimInstance->Montage_JumpToSection(sectionName, AttackMontage);
			}
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::Attack - AttackMontage is null.")); }
	}
	else if (!AnimInstance) { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::Attack - AnimInstance is null.")); }
}

void ASkeletonEnemy::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	AMyEnemy::OnNotifyBegin(NotifyName, BranchingPointPayload);
}
