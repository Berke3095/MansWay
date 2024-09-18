#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AMyShield;
class AMyWeapon;
class AMyCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MANSWAY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

	AMyCharacter* MyCharacter{};
	AMyShield* EquippedShield{};
	AMyWeapon* EquippedWeapon{};

	void GetReferences();
	void DropInteractable(AActor* actorToDrop);

public:

	void EquipInteractable(AActor* actorToEquip);
	FORCEINLINE bool CheckWeapons() { return EquippedWeapon && EquippedShield ? true : false; }
};
