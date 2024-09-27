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
			const float chaseResetTime{ 0.5f };
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

void AMyAIController::ChasePlayer(AMyEnemy* ownerEnemy)
{
	if (bCanChase)
	{
		if (MyCharacter && ownerEnemy)
		{
			MoveToActor(MyCharacter, ownerEnemy->GetAvoidance());

			FVector enemyLoc = ownerEnemy->GetActorLocation();
			FVector charLoc = MyCharacter->GetActorLocation();

			float distance = FVector::Dist(enemyLoc, charLoc);
			if (distance <= ownerEnemy->GetAttackRange())
			{
				ownerEnemy->SetEnemyCombatState(EEnemyCombatState::EECS_Attacking);
				if(ownerEnemy->GetbCanAttack()) 
				{ 
					ownerEnemy->Attack();
				}
			}
			else
			{
				ownerEnemy->SetEnemyCombatState(EEnemyCombatState::EECS_NONE);
			}
		}
	}
}
