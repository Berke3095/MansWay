#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/MyEnemyEnums.h"
#include "MyEnemy.generated.h"

class UCapsuleComponent;
class AMyAIController;
class AMyCharacter;
class UEnemyCombatComponent;
class UAnimMontage;
class UEnemyAnimInstance;

UCLASS(Abstract)
class MANSWAY_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AMyEnemy();
	virtual ~AMyEnemy() {}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
		ATTRIBS
	*/
	int32 MaxHP{};
	int32 HP{};

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
	float AttackCooldown{};

	 EEnemyCombatState EnemyCombatState{ EEnemyCombatState::EECS_NONE };

	/*
		ANIMATION
	*/
	UEnemyAnimInstance* AnimInstance{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage{};

	UFUNCTION()
	virtual void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

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

	FTimerHandle AttackTimer{};
	bool bCanAttack{ true };
	void ResetAttack();

	/*
		STATE
	*/
	EEnemyState EnemyState{ EEnemyState::ES_NONE };
	void SetEnemyState(EEnemyState enemyState);

	/*
		DEATH
	*/
	FTimerHandle DestroyInstanceTimer{};
	void DestroyDead();
	FTimerHandle BuryInstanceTimer{};
	void BuryDead(float deltaTime);
	bool bCanBury{};
	void ActivateBury() { bCanBury = true; }

	/*
		COMBAT
	*/
	FTimerHandle StunTimer{};
	void ResetStun();

public:
	FORCEINLINE const float GetEnemyYaw() const { return EnemyYaw; }
	FORCEINLINE const float GetEnemyPitch() const { return EnemyPitch; }
	FORCEINLINE const float GetEnemySpeed() const { return EnemySpeed; }

	FORCEINLINE const float GetAttackRange() const { return AttackRange; }
	FORCEINLINE const float GetAvoidance() const { return Avoidance; }

	FORCEINLINE const EEnemyCombatState GetEnemyCombatState() const { return EnemyCombatState; }
	FORCEINLINE void SetEnemyCombatState(EEnemyCombatState state) { if (EnemyCombatState != state) EnemyCombatState = state; }

	virtual void Attack() {};
	FORCEINLINE const bool GetbCanAttack() const { return bCanAttack; }

	FORCEINLINE const int32 GetHP() const { return HP; }
	FORCEINLINE void ReduceHP(int32 damage) { HP -= damage; }

	void GoDead();
	void GoStunned();
	
};
