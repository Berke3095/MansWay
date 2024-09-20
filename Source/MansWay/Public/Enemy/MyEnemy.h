#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyEnemy.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UArrowComponent;

UCLASS()
class MANSWAY_API AMyEnemy : public APawn
{
	GENERATED_BODY()

public:
	AMyEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	/*
		COMPONENTS
	*/
	void SetupComponents();

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent{};

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* MeshComponent{};

	UArrowComponent* ArrowComponent{};

};
