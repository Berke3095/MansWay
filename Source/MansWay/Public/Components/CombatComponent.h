#pragma once

#include "CoreMinimal.h"
#include "Components/BaseCombatComponent.h"
#include "CombatComponent.generated.h"

class AMyCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MANSWAY_API UCombatComponent : public UBaseCombatComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

private:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

	void SetupReferences();

	AMyCharacter* MyCharacter{};
};
