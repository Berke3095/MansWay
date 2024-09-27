#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/MyEnemyEnums.h"
#include "MyEnemy.generated.h"

class UCapsuleComponent;
class AMyAIController;
class AMyCharacter;
class UEnemyCombatComponent;

UCLASS(Abstract)
class MANSWAY_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AMyEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
		AI
	*/
	AMyAIController* MyAIController{};

	/*
		PLAYER
	*/
	AMyCharacter* MyCharacter{};

	/*
		OFFSET
	*/
	float YawLimit{};
	float PitchLimit{};

	/*
		ATTRIBS
	*/
	float DefaultSpeed{};

	UCapsuleComponent* CapsuleComponent{};
	USkeletalMeshComponent* MeshComponent{};
	UEnemyCombatComponent* CombatComponent{};

	/*
		ATTACK
	*/
	float AttackRange{};
	float Avoidance{};

private:

	void SetupReferences();

	/*
		COMPONENTS
	*/
	void SetupComponents();

	/*
		OFFSET
	*/
	float EnemyYaw{};
	float EnemyPitch{};

	void AimOffset(float deltaTime);
	bool bIsInterping{};
	float EnemySpeed{};

	/*
		ATTACK
	*/
	EEnemyCombatState EnemyCombatState = EEnemyCombatState::EECS_NONE;

public:
	FORCEINLINE const float GetEnemyYaw() const { return EnemyYaw; }
	FORCEINLINE const float GetEnemyPitch() const { return EnemyPitch; }
	FORCEINLINE const float GetEnemySpeed() const { return EnemySpeed; }

	FORCEINLINE const float GetAttackRange() const { return AttackRange; }
	FORCEINLINE const float GetAvoidance() const { return Avoidance; }

	FORCEINLINE const EEnemyCombatState GetEnemyCombatState() const { return EnemyCombatState; }
	FORCEINLINE void SetEnemyCombatState(EEnemyCombatState state) { if (EnemyCombatState != state) EnemyCombatState = state; }

};
