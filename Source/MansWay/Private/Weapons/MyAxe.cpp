#include "Weapons/MyAxe.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AMyAxe::AMyAxe()
{
	PrimaryActorTick.bCanEverTick = false;

	SetupComponents();
}

void AMyAxe::BeginPlay()
{
	Super::BeginPlay();
}

void AMyAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyAxe::SetupComponents()
{
	if (WeaponMesh)
	{
		WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyAxe::SetupComponents - WeaponMesh is null.")); }

	if (WeaponBox)
	{
		WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyAxe::SetupComponents - WeaponBox is null.")); }

	if (WeaponSphere)
	{
		WeaponSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyAxe::SetupComponents - WeaponSphere is null.")); }
}
