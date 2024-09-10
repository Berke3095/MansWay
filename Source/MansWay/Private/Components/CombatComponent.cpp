#include "Components/CombatComponent.h"
#include "Characters/MyCharacter.h"
#include "Weapons/MyShield.h"
#include "Weapons/MySword.h"
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

void UCombatComponent::DropInteractable(AActor* actorToDrop)
{
	if (MyCharacter && actorToDrop)
	{
		if (actorToDrop->IsAttachedTo(MyCharacter))
		{
			actorToDrop->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			actorToDrop->SetOwner(nullptr);

			if (AMyWeapon* DroppedWeapon = Cast<AMyWeapon>(actorToDrop)) { DroppedWeapon->SetDroppedSettings(); }
			else { UE_LOG(LogTemp, Error, TEXT("UCombatComponent::DropInteractable - MyCharacter is null.")); }
		}
	}
	else if (!MyCharacter) { UE_LOG(LogTemp, Error, TEXT("UCombatComponent::DropInteractable - MyCharacter is null.")); }
	else if (!actorToDrop) { UE_LOG(LogTemp, Error, TEXT("UCombatComponent::DropInteractable - actorToDrop is null.")); }
}

void UCombatComponent::EquipInteractable(AActor* actorToEquip)
{
	if (MyCharacter && actorToEquip)
	{
		if (actorToEquip->IsA<AMyShield>())
		{
			if (EquippedShield) { DropInteractable(EquippedShield); }

			if (const USkeletalMeshSocket* ShieldSocket = MyCharacter->GetMesh()->GetSocketByName(FName("Shield_Socket")))
			{
				EquippedShield = Cast<AMyShield>(actorToEquip);
				EquippedShield->SetOwner(MyCharacter);
				EquippedShield->SetEquippedSettings();

				ShieldSocket->AttachActor(actorToEquip, MyCharacter->GetMesh());
			}
			else { UE_LOG(LogTemp, Error, TEXT("UCombatComponent::EquipInteractable - Shield_socket is null.")); }
		}
		else if (actorToEquip->IsA<AMySword>())
		{
			if (EquippedWeapon) { DropInteractable(EquippedWeapon); }

			if (const USkeletalMeshSocket* SwordSocket = MyCharacter->GetMesh()->GetSocketByName(FName("Sword_Socket")))
			{
				EquippedWeapon = Cast<AMySword>(actorToEquip);
				EquippedWeapon->SetOwner(MyCharacter);
				EquippedWeapon->SetEquippedSettings();

				SwordSocket->AttachActor(actorToEquip, MyCharacter->GetMesh());
			}
			else { UE_LOG(LogTemp, Error, TEXT("UCombatComponent::EquipInteractable - Axe_socket is null.")); }
		}

		if (MyCharacter->GetClosestInteractable() == actorToEquip) { MyCharacter->SetClosestInteractable(nullptr); }
	}
	else if (!MyCharacter) { UE_LOG(LogTemp, Error, TEXT("UCombatComponent::EquipShield - MyCharacter is null.")); }
	else if (!actorToEquip) { UE_LOG(LogTemp, Error, TEXT("UCombatComponent::EquipShield - actorToEquip is null.")); }
}

