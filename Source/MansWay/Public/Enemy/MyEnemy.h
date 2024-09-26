#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/MyEnemyEnums.h"
#include "MyEnemy.generated.h"

class UCapsuleComponent;
class AMyAIController;
class AMyCharacter;
class UEnemyCombatComponent;
class USphereComponent;

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

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* AttackSphere{};

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
	UFUNCTION()
	void OnEnemySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	EEnemyCombatState EnemyCombatState = EEnemyCombatState::EECS_NONE;

public:
	FORCEINLINE const float GetEnemyYaw() const { return EnemyYaw; }
	FORCEINLINE const float GetEnemyPitch() const { return EnemyPitch; }
	FORCEINLINE const float GetEnemySpeed() const { return EnemySpeed; }

};
