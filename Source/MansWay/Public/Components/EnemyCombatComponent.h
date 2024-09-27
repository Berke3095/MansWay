#pragma once

#include "CoreMinimal.h"
#include "Components/BaseCombatComponent.h"
#include "EnemyCombatComponent.generated.h"

class AMyEnemy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MANSWAY_API UEnemyCombatComponent : public UBaseCombatComponent
{
	GENERATED_BODY()

public:	
	UEnemyCombatComponent();

private:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupReferences();

	AMyEnemy* Enemy{};

};
