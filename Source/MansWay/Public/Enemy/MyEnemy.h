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

	EEnemyCombatState EnemyCombatState = EEnemyCombatState::EECS_NONE;

	/*
		ANIMATION
	*/
	UEnemyAnimInstance* AnimInstance{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage{};

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
};
