#include "Components/BaseCombatComponent.h"
#include "Weapons/MyShield.h"
#include "Weapons/MySword.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Characters/MyCharacter.h"

UBaseCombatComponent::UBaseCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UBaseCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBaseCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBaseCombatComponent::DropInteractable(ACharacter* owner, AActor* actorToDrop)
{
	if (owner && actorToDrop)
	{
		if (actorToDrop->IsAttachedTo(owner))
		{
			actorToDrop->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			actorToDrop->SetOwner(nullptr);

			if (actorToDrop->IsA<AMyWeapon>())
			{
				AMyWeapon* droppedWeapon = Cast<AMyWeapon>(actorToDrop);
				if (droppedWeapon) { droppedWeapon->SetDroppedSettings(); }
			}
			else { UE_LOG(LogTemp, Error, TEXT("UBaseCombatComponent::DropInteractable - Dropped object is unknown.")); }
		}
	}
	else if (!owner) { UE_LOG(LogTemp, Error, TEXT("UBaseCombatComponent::DropInteractable - owner is null.")); }
	else if (!actorToDrop) { UE_LOG(LogTemp, Error, TEXT("UBaseCombatComponent::DropInteractable - actorToDrop is null.")); }
}

void UBaseCombatComponent::EquipInteractable(ACharacter* newOwner, AActor* actorToEquip)
{
	if (newOwner && actorToEquip)
	{
		if (actorToEquip->IsA<AMyShield>())
		{
			if (EquippedLeftHand) { DropInteractable(newOwner, EquippedLeftHand); }

			if (const USkeletalMeshSocket* shieldSocket = newOwner->GetMesh()->GetSocketByName(FName("Shield_Socket")))
			{
				EquippedLeftHand = Cast<AMyShield>(actorToEquip);
				EquippedLeftHand->SetOwner(newOwner);
				EquippedLeftHand->SetEquippedSettings();

				shieldSocket->AttachActor(actorToEquip, newOwner->GetMesh());
			}
			else { UE_LOG(LogTemp, Error, TEXT("UBaseCombatComponent::EquipInteractable - Shield_socket is null.")); }
		}
		else if (actorToEquip->IsA<AMySword>())
		{
			if (EquippedRightHand) { DropInteractable(newOwner, EquippedRightHand); }

			if (const USkeletalMeshSocket* swordSocket = newOwner->GetMesh()->GetSocketByName(FName("Sword_Socket")))
			{
				EquippedRightHand = Cast<AMySword>(actorToEquip);
				EquippedRightHand->SetOwner(newOwner);
				EquippedRightHand->SetEquippedSettings();

				swordSocket->AttachActor(actorToEquip, newOwner->GetMesh());
			}
			else { UE_LOG(LogTemp, Error, TEXT("UBaseCombatComponent::EquipInteractable -Sword_socket is null.")); }
		}

		if (newOwner->IsA<AMyCharacter>())
		{
			
			if(AMyCharacter* character = Cast<AMyCharacter>(newOwner))
			{
				if(character->GetClosestInteractable() == actorToEquip) { character->SetClosestInteractable(nullptr); }
			}
			else { UE_LOG(LogTemp, Error, TEXT("UBaseCombatComponent::EquipInteractable - character is null.")); }
		}
	}
	else if (!newOwner) { UE_LOG(LogTemp, Error, TEXT("UBaseCombatComponent::EquipInteractable - newOwner is null.")); }
	else if (!actorToEquip) { UE_LOG(LogTemp, Error, TEXT("UBaseCombatComponent::EquipInteractable - actorToEquip is null.")); }
}

void UBaseCombatComponent::EnableRightWeapon()
{
	if (EquippedRightHand)
	{
		EquippedRightHand->EnableWeaponBox();
	}
}

void UBaseCombatComponent::EnableLeftWeapon()
{
	if (EquippedLeftHand)
	{
		EquippedLeftHand->EnableWeaponBox();
	}
}

void UBaseCombatComponent::DisableRightWeapon()
{
	if (EquippedRightHand)
	{
		EquippedRightHand->DisableWeaponBox();
	}
}

void UBaseCombatComponent::DisableLeftWeapon()
{
	if (EquippedLeftHand)
	{
		EquippedLeftHand->DisableWeaponBox();
	}
}

