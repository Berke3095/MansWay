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

};
