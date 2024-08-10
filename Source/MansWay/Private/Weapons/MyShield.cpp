#include "Weapons/MyShield.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/MyCharacter.h"

AMyShield::AMyShield()
{
	PrimaryActorTick.bCanEverTick = true;

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
		WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Shield type
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - WeaponMesh is null.")); }

	if (WeaponBox)
	{
		WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Shield type
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - WeaponBox is null.")); }

	if (WeaponSphere)
	{
		WeaponSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Shield type
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - WeaponSphere is null.")); }
}