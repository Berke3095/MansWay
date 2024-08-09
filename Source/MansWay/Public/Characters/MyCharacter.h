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
		ATTRIBUTES
	*/
	float DefaultSpeed{ 250.0f };
	float SprintSpeed{ 450.0f };

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
	const float StartingTargetArmLength{ 100.0f };
	const FVector StartingSocketOffset{ FVector(0.0f, 60.0f, 45.0f) };

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera{};

	/*
		INPUT
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* CharacterMappingContext{};

	void Move(const FInputActionValue& InputValue1);
	void StopMove();
	void Look(const FInputActionValue& InputValue1);
	void BasicAttack();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* BasicAttackAction{};
	
	float Speed{};
	void UseControllerYaw(float DeltaTime1);

	/*
		OFFSET
	*/
	float CharacterYaw{};
	float CharacterPitch{};

	void AimOffset(float DeltaTime1);


private:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	FORCEINLINE const float GetCharacterYaw() const { return CharacterYaw; }
	FORCEINLINE const float GetCharacterPitch() const { return CharacterPitch; }
	
	FORCEINLINE const float GetSpeed() const { return Speed; }
};
