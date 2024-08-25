#include "Weapons/MySword.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AMySword::AMySword()
{
	PrimaryActorTick.bCanEverTick = false;

	SetupComponents();
}

void AMySword::BeginPlay()
{
	Super::BeginPlay();

	SetupComponents();
}

void AMySword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMySword::SetupComponents()
{
	if (WeaponMesh)
	{
		WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMySword::SetupComponents - WeaponMesh is null.")); }

	if (WeaponBox)
	{
		WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMySword::SetupComponents - WeaponBox is null.")); }

	if (WeaponSphere)
	{
		WeaponSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMySword::SetupComponents - WeaponSphere is null.")); }
}
