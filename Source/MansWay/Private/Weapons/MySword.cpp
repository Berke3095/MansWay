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

}
