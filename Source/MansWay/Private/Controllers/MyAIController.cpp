#include "Controllers/MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/MyCharacter.h"
#include "Enemy/MyEnemy.h"
#include "Navigation/PathFollowingComponent.h"

AMyAIController::AMyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!MyCharacter) { UE_LOG(LogTemp, Error, TEXT("AMyAIController::BeginPlay - MyCharacter is null")); }

	MyEnemy = Cast<AMyEnemy>(GetPawn());
	if (!MyEnemy) { UE_LOG(LogTemp, Error, TEXT("AMyAIController::BeginPlay - MyEnemy is null")); }
}

void AMyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsSuccess())
	{
		if (!GetWorldTimerManager().IsTimerActive(ChaseTimer))
		{
			bCanChase = false;
			float chaseResetTime{ 0.5f };
			GetWorldTimerManager().SetTimer(ChaseTimer, this, &AMyAIController::ResetChase, chaseResetTime, false);
		}
	}
}

void AMyAIController::ResetChase()
{
	if (GetWorldTimerManager().IsTimerActive(ChaseTimer))
	{
		GetWorldTimerManager().ClearTimer(ChaseTimer);
	}
	bCanChase = true;
}

void AMyAIController::ChasePlayer()
{
	if (bCanChase)
	{
		if (MyCharacter && MyEnemy)
		{
			MoveToActor(MyCharacter, MyEnemy->GetAvoidance());

			FVector enemyLoc = MyEnemy->GetActorLocation();
			FVector charLoc = MyCharacter->GetActorLocation();

			float distance = FVector::Dist(enemyLoc, charLoc);
			if (distance <= MyEnemy->GetAttackRange())
			{
				MyEnemy->SetEnemyCombatState(EEnemyCombatState::EECS_Attacking);
			}
			else
			{
				MyEnemy->SetEnemyCombatState(EEnemyCombatState::EECS_NONE);
			}
		}
	}
}
