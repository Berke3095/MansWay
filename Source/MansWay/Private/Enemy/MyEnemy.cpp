#include "Enemy/MyEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Controllers/MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/EnemyCombatComponent.h"
#include "Animation/AnimMontage.h"
#include "Animations/EnemyAnimInstance.h"
#include "Weapons/MyWeapon.h"

AMyEnemy::AMyEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	SetupComponents();
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetupReferences();
}

void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState == EEnemyState::ES_NONE)
	{
		AimOffset(DeltaTime);

		if (MyAIController)
		{
			MyAIController->ChasePlayer(this);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::Tick - MyAIController is null")); }
	}
	
	if (bCanBury) { BuryDead(DeltaTime); }
}

void AMyEnemy::SetupReferences()
{
	MyAIController = Cast<AMyAIController>(GetController());
	if (!MyAIController) { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::SetupReferences - MyAIController is null")); }

	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(!MyCharacter) { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::SetupReferences - MyCharacter is null")); }

	AnimInstance = Cast<UEnemyAnimInstance>(MeshComponent->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AMyEnemy::OnNotifyBegin);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::SetupReferences - AnimInstance is null.")); }
}

void AMyEnemy::SetupComponents()
{
	CapsuleComponent = GetCapsuleComponent();
	if (CapsuleComponent)
	{
		RootComponent = CapsuleComponent;
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Overlap); // Enemy targeter
		CapsuleComponent->SetGenerateOverlapEvents(true);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::SetupComponents - CapsuleComponent is null.")); }

	MeshComponent = GetMesh();
	if (MeshComponent)
	{
		MeshComponent->SetupAttachment(RootComponent);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4); // Enemy
		MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap); // Player weapon
		MeshComponent->SetGenerateOverlapEvents(true);
		MeshComponent->bUseAttachParentBound = true;
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::SetupComponents - MeshComponent is null.")); }

	CombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
	if(!CombatComponent){ UE_LOG(LogTemp, Error, TEXT("AMyEnemy::SetupComponents - CombatComponent is null.")); }
}

void AMyEnemy::AimOffset(float deltaTime)
{
	if (MyCharacter && MyCharacter->GetMesh())
	{
		FVector characterHeadLocation = MyCharacter->GetMesh()->GetBoneLocation("head");
		FVector enemyHeadLocation = MeshComponent->GetBoneLocation("head");

		FVector directionToPlayer = (characterHeadLocation - enemyHeadLocation).GetSafeNormal();
		FRotator aimRotation = directionToPlayer.Rotation();

		FRotator currentRotation = GetActorRotation();
		FRotator deltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(aimRotation, currentRotation);

		EnemySpeed = UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity);
		if (EnemySpeed < KINDA_SMALL_NUMBER)
		{
			if (FMath::Abs(deltaRotation.Yaw) > YawLimit || bIsInterping)
			{
				FRotator interpolatedRotation = FMath::RInterpTo(currentRotation, aimRotation, deltaTime, 4.0f);
				SetActorRotation(interpolatedRotation);

				deltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(aimRotation, interpolatedRotation);

				bIsInterping = !deltaRotation.IsNearlyZero(30.0f);
			}
		}

		EnemyYaw = deltaRotation.Yaw;
		EnemyPitch = deltaRotation.Pitch;
	}
}

void AMyEnemy::ResetAttack()
{
	if (GetWorldTimerManager().IsTimerActive(AttackTimer))
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}
	bCanAttack = true;
}

void AMyEnemy::SetEnemyState(EEnemyState enemyState)
{
	if (AnimInstance)
	{
		AnimInstance->SetEnemyState(enemyState);
		if (EnemyState != enemyState)
		{
			EnemyState = enemyState;
			if (enemyState != EEnemyState::ES_NONE)
			{
				GetCharacterMovement()->DisableMovement();
			}
			else
			{
				GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
			}
		}
	}
}

void AMyEnemy::GoDead()
{
	ResetStun();
	if (AnimInstance && DeathMontage)
	{
		SetEnemyState(EEnemyState::ES_Dead);

		if (!AnimInstance->Montage_IsPlaying(DeathMontage))
		{
			AnimInstance->Montage_Play(DeathMontage);
		}
	}
	else if (!AnimInstance) { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::GoDead - AnimInstance is null.")); }
	else if (!DeathMontage) { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::GoDead - DeathMontage is null.")); }

	if (!GetWorldTimerManager().IsTimerActive(BuryInstanceTimer))
	{
		float timeUntilLowering = 2.0f;
		GetWorldTimerManager().SetTimer(BuryInstanceTimer, this, &AMyEnemy::ActivateBury, timeUntilLowering, false);
	}

	if (CombatComponent)
	{
		if (CombatComponent->GetLeftWeapon())
		{
			CombatComponent->DropInteractable(this, Cast<AActor>(CombatComponent->GetLeftWeapon()));
		}

		if (CombatComponent->GetRightWeapon())
		{
			CombatComponent->DropInteractable(this, Cast<AActor>(CombatComponent->GetRightWeapon()));
		}
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::GoDead - CombatComponent is null.")); }

	if (CapsuleComponent) { CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); }
	if (MeshComponent) { MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); }
}

void AMyEnemy::GoStunned()
{
	if (AnimInstance)
	{
		SetEnemyState(EEnemyState::ES_Stunned);
		if (!GetWorldTimerManager().IsTimerActive(StunTimer))
		{
			float stunCooldown = 2.0f;
			GetWorldTimerManager().SetTimer(StunTimer, this, &AMyEnemy::ResetStun, stunCooldown, false);
		}
	}
}

void AMyEnemy::DestroyDead()
{
	if (GetWorldTimerManager().IsTimerActive(DestroyInstanceTimer))
	{
		GetWorldTimerManager().ClearTimer(DestroyInstanceTimer);
	}
	Destroy();
}

void AMyEnemy::BuryDead(float deltaTime)
{
	float lowerSpeed = 5.0f;

	FVector currentLocation = GetActorLocation();
	currentLocation.Z -= lowerSpeed * deltaTime;

	SetActorLocation(currentLocation);

	if (!GetWorldTimerManager().IsTimerActive(DestroyInstanceTimer))
	{
		float timeUntilDestroy = 10.0f;
		GetWorldTimerManager().SetTimer(DestroyInstanceTimer, this, &AMyEnemy::DestroyDead, timeUntilDestroy, false);
	}
}

void AMyEnemy::ResetStun()
{
	if (GetWorldTimerManager().IsTimerActive(StunTimer))
	{
		GetWorldTimerManager().ClearTimer(StunTimer);
	}
	SetEnemyState(EEnemyState::ES_NONE);
}

void AMyEnemy::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == "Cooldown_Initiator")
	{
		bCanAttack = false;
		if (!GetWorldTimerManager().IsTimerActive(AttackTimer))
		{
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AMyEnemy::ResetAttack, AttackCooldown, false);
		}
	}

	if (CombatComponent)
	{
		if (NotifyName == "Right_Collision_Enable")
		{
			CombatComponent->EnableRightWeapon();
		}
		else if (NotifyName == "Right_Collision_Disable")
		{
			CombatComponent->DisableRightWeapon();
		}

		if (NotifyName == "Left_Collision_Enable")
		{
			CombatComponent->EnableLeftWeapon();
		}
		else if (NotifyName == "Left_Collision_Disable")
		{
			CombatComponent->DisableLeftWeapon();
		}
	}
}