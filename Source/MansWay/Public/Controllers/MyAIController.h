#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

class AMyCharacter;
class AMyEnemy;

UCLASS()
class MANSWAY_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyAIController();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	AMyCharacter* MyCharacter{};
	AMyEnemy* MyEnemy{};

public:
	void ChasePlayer();

};
