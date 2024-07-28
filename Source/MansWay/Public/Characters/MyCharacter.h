#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

// Input
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

// Components
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class MANSWAY_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
		REFERENCES
	*/
	void SetupReferences();
	APlayerController* PlayerController{};

	/*
		COMPONENTS
	*/
	void SetupComponents();

	UCapsuleComponent* CapsuleComponent{};
	USkeletalMeshComponent* MeshComponent{};

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm{};
	const float StartingTargetArmLength{ 300.0f };
	const FVector StartingSocketOffset{ FVector(0.0f, 50.0f, 50.0f) };

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera{};

	/*
		INPUT
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* CharacterMappingContext{};

	void Move(const FInputActionValue& InputValue1);
	void StopMove();
	float MoveDirection{};
	float Speed{ 0.0f };
	void Look(const FInputActionValue& InputValue1);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction{};
	
	void UseControllerYaw(float DeltaTime1);

private:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	FORCEINLINE const float GetMoveDirection() const { return MoveDirection; }
	FORCEINLINE const float GetSpeed() const { return Speed; }
};
