#include "Components/CombatComponent.h"
#include "Characters/MyCharacter.h"
#include "Weapons/MyShield.h"
#include "Engine/SkeletalMeshSocket.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	GetReferences();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::GetReferences()
{
	MyCharacter = Cast<AMyCharacter>(GetOwner());
	if(!MyCharacter) { UE_LOG(LogTemp, Error, TEXT("UCombatComponent::GetReferences - MyCharacter is null.")); }
}

void UCombatComponent::EquipShield(AMyShield* ShieldToEquip1)
{
	if (MyCharacter && ShieldToEquip1)
	{
		if (const USkeletalMeshSocket* ShieldSocket = MyCharacter->GetMesh()->GetSocketByName(FName("Shield_socket")))
		{
			ShieldSocket->AttachActor(ShieldToEquip1, MyCharacter->GetMesh());
		}
		else { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipShield - ShieldSocket is null.")); }

		EquippedShield = ShieldToEquip1;
		EquippedShield->SetOwner(MyCharacter);
		EquippedShield->SetEquippedSettings();
	}
	else if (!MyCharacter) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipShield - MyCharacter is null.")); }
	else if (!ShieldToEquip1) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipShield - ShieldToEquip1 is null.")); }
}

