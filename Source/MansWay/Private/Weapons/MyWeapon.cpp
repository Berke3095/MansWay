#include "Weapons/MyWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/MyCharacter.h"
#include "Widgets/MyWidgetManager.h"
#include "Enemy/MyEnemy.h"
#include "Animations/MyAnimInstance.h"

AMyWeapon::AMyWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SetupComponents();
}

void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	WidgetManager = AMyWidgetManager::GetInstance();
	if(!WidgetManager) { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::BeginPlay - WidgetManager is null.")); }
}

void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMyCharacter>())
	{
		MyCharacter = Cast<AMyCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->AddToOverlaps(this);
			WidgetManager->CreatePickUpWidget();
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::OnSphereOverlap - MyCharacter is null.")); }
	}
}

void AMyWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AMyCharacter>())
	{
		if (MyCharacter)
		{
			MyCharacter->RemoveFromOverlaps(this);
			if (MyCharacter->GetOverlappingInteractables().Num() == 0)
			{
				WidgetManager->RemovePickUpWidget();
			}
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::OnSphereEndOverlap - MyCharacter is null.")); }
	}
}

void AMyWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMyEnemy>())
	{
		DisableWeaponBox();
		if (AMyEnemy* enemy = Cast<AMyEnemy>(OtherActor))
		{
			int32 random = FMath::RandRange(-10, 10);
			int32 damage = WeaponDamage + random;
			enemy->ReduceHP(damage);

			if (enemy->GetHP() <= 0)
			{
				enemy->GoDead();
			}
		}
	}
	else if(OtherActor->IsA<AMyCharacter>())
	{
		DisableWeaponBox();
		if (AMyCharacter* character = Cast<AMyCharacter>(OtherActor))
		{
			if (character->GetCombatState() == ECombatState::ECS_Parrying && character->GetLockedEnemy() == this->GetOwner())
			{
				if (UMyAnimInstance* characterAnimInstance = character->GetAnimInstance())
				{
					UAnimMontage* parryMontage = character->GetParryMontage();
					characterAnimInstance->Montage_Play(parryMontage);
					characterAnimInstance->Montage_JumpToSection("Parry_Hit", parryMontage);
					character->SetCombatState(ECombatState::ECS_NONE);
				}
				if (AMyEnemy* enemy = Cast<AMyEnemy>(this->GetOwner()))
				{
					enemy->GoStunned();
				}
			}
			else
			{
				int32 random = FMath::RandRange(-10, 10);
				int32 damage = WeaponDamage + random;
				character->ReduceHP(damage);
			}
		}
	}
}

void AMyWeapon::SetupComponents()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	if (WeaponMesh)
	{
		RootComponent = WeaponMesh;
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel6); // NeutralItem
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetupComponents - WeaponMesh is null.")); }

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	if (WeaponBox)
	{
		WeaponBox->SetupAttachment(WeaponMesh);
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel6); // NeutralItem
		WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnBoxOverlap);
		WeaponBox->SetGenerateOverlapEvents(true);
		WeaponBox->bUseAttachParentBound = true;
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetupComponents - WeaponBox is null.")); }

	WeaponSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Weapon Sphere"));
	if (WeaponSphere)
	{
		WeaponSphere->SetupAttachment(WeaponMesh);
		WeaponSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel6); // NeutralItem
		WeaponSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		WeaponSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnSphereOverlap);
		WeaponSphere->OnComponentEndOverlap.AddDynamic(this, &AMyWeapon::OnSphereEndOverlap);
		WeaponSphere->bUseAttachParentBound = true;
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetupComponents - WeaponSphere is null.")); }
}

void AMyWeapon::TurnOnPhysics()
{
	if (WeaponMesh)
	{
		const float TimeToTurnOffPhysics{ 5.0f };
		GetWorldTimerManager().SetTimer(TurnOffPhysicsTimer, this, &AMyWeapon::TurnOffPhysics, TimeToTurnOffPhysics, false);

		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetSimulatePhysics(true);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::TurnOnPhysics - WeaponMesh is null.")); }
}

void AMyWeapon::TurnOffPhysics()
{
	if (WeaponMesh)
	{
		if (GetWorldTimerManager().IsTimerActive(TurnOffPhysicsTimer))
		{
			GetWorldTimerManager().ClearTimer(TurnOffPhysicsTimer);
		}

		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::TurnOffPhysics - WeaponMesh is null.")); }
}

void AMyWeapon::SetEquippedSettings()
{
	TurnOffPhysics();

	if (WeaponSphere)
	{
		WeaponSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetEquippedSettings - WeaponSphere is null.")); }

	if (WeaponBox)
	{
		WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		if (GetOwner()->IsA<AMyCharacter>())
		{
			WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
			WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Overlap); // Enemy
		}
		else if(GetOwner()->IsA<AMyEnemy>())
		{
			WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2); // Enemy weapon
			WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap); // Player
		}
		else { UE_LOG(LogTemp, Error, TEXT("Weapon equipped by unknown owner.")); }
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetEquippedSettings - WeaponBox is null.")); }
}

void AMyWeapon::SetDroppedSettings()
{
	TurnOnPhysics();

	if (WeaponSphere)
	{
		WeaponSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetDroppedSettings - WeaponSphere is null.")); }

	if (WeaponBox)
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel6); // NeutralItem
		WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetDroppedSettings - WeaponBox is null.")); }
}

void AMyWeapon::EnableWeaponBox()
{
	if (WeaponBox) { WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); }
}

void AMyWeapon::DisableWeaponBox()
{
	if (WeaponBox) { WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); }
}

