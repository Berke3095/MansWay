#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseCombatComponent.generated.h"

class AMyWeapon;

UCLASS(ClassGroup = (Custom), Abstract)
class MANSWAY_API UBaseCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBaseCombatComponent();
	virtual ~UBaseCombatComponent() {}

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AMyWeapon* EquippedLeftHand{};
	AMyWeapon* EquippedRightHand{};

public:
	void DropInteractable(ACharacter* owner, AActor* actorToDrop);
	void EquipInteractable(ACharacter* newOwner, AActor* actorToEquip);
	FORCEINLINE bool CheckWeapons() { return EquippedRightHand && EquippedLeftHand ? true : false; }

	FORCEINLINE AMyWeapon* GetLeftWeapon() const { return EquippedLeftHand ? EquippedLeftHand : nullptr; }
	FORCEINLINE AMyWeapon* GetRightWeapon() const { return EquippedRightHand ? EquippedRightHand : nullptr; }

	void EnableRightWeapon();
	void EnableLeftWeapon();
	void DisableRightWeapon();
	void DisableLeftWeapon();
};
