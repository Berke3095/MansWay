#include "Controllers/MyAIController.h"

AMyAIController::AMyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
