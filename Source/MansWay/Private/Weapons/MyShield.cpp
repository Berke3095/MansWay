#include "Weapons/MyShield.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AMyShield::AMyShield()
{
	PrimaryActorTick.bCanEverTick = false;

	SetupComponents();
}

void AMyShield::BeginPlay()
{
	Super::BeginPlay();
}

void AMyShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

void AMyShield::SetupComponents()
{
	if (WeaponMesh)
	{
		WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - WeaponMesh is null.")); }

	if (WeaponBox)
	{
		WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - WeaponBox is null.")); }

	if (WeaponSphere)
	{
		WeaponSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - WeaponSphere is null.")); }
}