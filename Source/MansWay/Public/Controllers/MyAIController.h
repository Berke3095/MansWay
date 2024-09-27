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

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	FTimerHandle ChaseTimer{};
	bool bCanChase{ true };
	void ResetChase();

public:
	void ChasePlayer(AMyEnemy* ownerEnemy);

};
