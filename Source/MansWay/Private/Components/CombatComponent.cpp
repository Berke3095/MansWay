#include "Components/CombatComponent.h"
#include "Characters/MyCharacter.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupReferences();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::SetupReferences()
{
	MyCharacter = Cast<AMyCharacter>(GetOwner());
	if(!MyCharacter) { UE_LOG(LogTemp, Error, TEXT("UCombatComponent::GetReferences - MyCharacter is null.")); }
}

