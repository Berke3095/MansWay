#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AMyShield;
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

	void GetReferences();
	void DropInteractable(AActor* ActorToDrop1);

public:

	void EquipInteractable(AActor* ActorToEquip1);
};
