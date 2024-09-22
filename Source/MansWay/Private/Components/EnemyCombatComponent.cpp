#include "Components/EnemyCombatComponent.h"
#include "Enemy/MyEnemy.h"

UEnemyCombatComponent::UEnemyCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetupReferences();
}

void UEnemyCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEnemyCombatComponent::SetupReferences()
{
	Enemy = Cast<AMyEnemy>(GetOwner());
	if (!Enemy) { UE_LOG(LogTemp, Error, TEXT("UEnemyCombatComponent::GetReferences - Enemy is null.")); }
}

