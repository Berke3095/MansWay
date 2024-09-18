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
	
}