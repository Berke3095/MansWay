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
	
}
