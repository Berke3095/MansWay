#include "Enemy/MyEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Controllers/MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/EnemyCombatComponent.h"

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

	AimOffset(DeltaTime);
}

void AMyEnemy::SetupReferences()
{
	MyAIController = Cast<AMyAIController>(GetController());
	if (!MyAIController) { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::SetupReferences - MyAIController is null")); }

	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(!MyCharacter) { UE_LOG(LogTemp, Error, TEXT("AMyEnemy::SetupReferences - MyCharacter is null")); }
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
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Overlap); // NeutralItem
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Overlap); // Enemy targeter
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
				FRotator interpolatedRotation = FMath::RInterpTo(currentRotation, aimRotation, deltaTime, 2.0f);
				SetActorRotation(interpolatedRotation);

				deltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(aimRotation, interpolatedRotation);

				bIsInterping = !deltaRotation.IsNearlyZero(30.0f);
			}
		}

		EnemyYaw = deltaRotation.Yaw;
		EnemyPitch = deltaRotation.Pitch;
	}
}
