#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"

class UCapsuleComponent;
class AMyAIController;
class AMyCharacter;

UCLASS()
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

private:

	void SetupReferences();

	/*
		COMPONENTS
	*/
	void SetupComponents();

	UCapsuleComponent* CapsuleComponent{};
	USkeletalMeshComponent* MeshComponent{};

	/*
		OFFSET
	*/
	float EnemyYaw{};
	float EnemyPitch{};

	void AimOffset(float deltaTime);
	bool bIsInterping{};
	float EnemySpeed{};

protected:
	float YawLimit{};
	float PitchLimit{};

	/*
		ATTRIBS
	*/
	float DefaultSpeed{};
	

public:
	FORCEINLINE const float GetEnemyYaw() const { return EnemyYaw; }
	FORCEINLINE const float GetEnemyPitch() const { return EnemyPitch; }
	FORCEINLINE const float GetEnemySpeed() const { return EnemySpeed; }

};
