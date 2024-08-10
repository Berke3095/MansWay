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

void UCombatComponent::EquipInteractable(AActor* ActorToEquip1)
{
	if (MyCharacter && ActorToEquip1)
	{
		if (ActorToEquip1->IsA<AMyShield>())
		{
			if (const USkeletalMeshSocket* ShieldSocket = MyCharacter->GetMesh()->GetSocketByName(FName("Shield_socket")))
			{
				ShieldSocket->AttachActor(ActorToEquip1, MyCharacter->GetMesh());
			}
			else { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipShield - ShieldSocket is null.")); }

			EquippedShield = Cast<AMyShield>(ActorToEquip1);
			EquippedShield->SetOwner(MyCharacter);
			EquippedShield->SetEquippedSettings();
		}
	}
	else if (!MyCharacter) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipShield - MyCharacter is null.")); }
	else if (!ActorToEquip1) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipShield - ActorToEquip1 is null.")); }
}

