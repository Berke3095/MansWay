#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

public:
	FORCEINLINE const float GetEnemyYaw() const { return EnemyYaw; }
	FORCEINLINE const float GetEnemyPitch() const { return EnemyPitch; }
	FORCEINLINE const float GetEnemySpeed() const { return EnemySpeed; }

};
