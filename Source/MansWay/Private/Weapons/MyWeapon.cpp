#include "Weapons/MyWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/MyCharacter.h"
#include "Widgets/MyWidgetManager.h"

AMyWeapon::AMyWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

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

void AMyWeapon::SetupComponents()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	if (WeaponMesh)
	{
		RootComponent = WeaponMesh;
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetupComponents - WeaponMesh is null.")); }

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	if (WeaponBox)
	{
		WeaponBox->SetupAttachment(WeaponMesh);
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnSphereOverlap);
		WeaponBox->OnComponentEndOverlap.AddDynamic(this, &AMyWeapon::OnSphereEndOverlap);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetupComponents - WeaponBox is null.")); }

	WeaponSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Weapon Sphere"));
	if (WeaponSphere)
	{
		WeaponSphere->SetupAttachment(WeaponMesh);
		WeaponSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		WeaponSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnSphereOverlap);
		WeaponSphere->OnComponentEndOverlap.AddDynamic(this, &AMyWeapon::OnSphereEndOverlap);
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
		// WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5); // PhysicsTool
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
		// WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::TurnOffPhysics - WeaponMesh is null.")); }
}

void AMyWeapon::SetEquippedSettings()
{
	TurnOffPhysics();

	if (WeaponSphere)
	{
		WeaponSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetEquippedSettings - WeaponSphere is null.")); }

	if (WeaponBox)
	{
		WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		if (GetOwner()->IsA<AMyCharacter>())
		{
			WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Overlap); // Enemy
		}
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
		WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyWeapon::SetDroppedSettings - WeaponBox is null.")); }
}

