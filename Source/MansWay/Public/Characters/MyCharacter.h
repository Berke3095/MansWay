#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UCombatComponent;
class UMyAnimInstance;


UCLASS()
class MANSWAY_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Movement(float DeltaTime1);

	/*
		ATTRIBUTES
	*/
	float DefaultSpeed{ 250.0f };
	float CombatSpeed{ 125.0 };
	float SprintSpeed{ 350.0f };

	/*
		REFERENCES
	*/
	void SetupReferences();
	APlayerController* PlayerController{};
	AActor* ClosestInteractable{};

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

	UCombatComponent* CombatComponent{};

	void SwitchStanceCamera(float DeltaTime1);

	/*
		INPUT
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* CharacterMappingContext{};

	void Move(const FInputActionValue& InputValue1);
	void StopMove();
	void Look(const FInputActionValue& InputValue1);
	void BasicAttack();
	void Interact();
	void StanceSwitch();
	void Parry();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* BasicAttackAction{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractAction{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* StanceSwitchAction{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ParryAction{};
	
	float Speed{};
	void UseControllerYaw(float DeltaTime1);

	/*
		OFFSET
	*/
	float CharacterYaw{};
	float CharacterPitch{};
	void AimOffset(float DeltaTime1);

	TArray<AActor*> Overlaps{};

	/*
		ANIMATION
	*/
	UMyAnimInstance* MyAnimInstance{};

	bool bCombatStance{}; // Combat mode if true
	FTimerHandle CameraInterpTimer{};
	void InterpCamera();
	void StopCameraInterp();
	bool bInterpInProcess{};

private:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	FORCEINLINE const float GetCharacterYaw() const { return CharacterYaw; }
	FORCEINLINE const float GetCharacterPitch() const { return CharacterPitch; }
	
	FORCEINLINE const float GetSpeed() const { return Speed; }

	FORCEINLINE void SetClosestInteractable(AActor* ClosestInteractable1) { ClosestInteractable = ClosestInteractable1; }
	FORCEINLINE const AActor* GetClosestInteractable() const { return ClosestInteractable ? ClosestInteractable : nullptr; }

	FORCEINLINE void AddToOverlaps(AActor* InteractableToAdd1) { if (!Overlaps.Contains(InteractableToAdd1)) { Overlaps.Add(InteractableToAdd1); } }
	FORCEINLINE void RemoveFromOverlaps(AActor* InteractableToRemove1) { if (Overlaps.Contains(InteractableToRemove1)) { Overlaps.Remove(InteractableToRemove1); } }
	FORCEINLINE TArray<AActor*> GetOverlappingInteractables() const { return Overlaps; }
	AActor* CalculateClosestOverlap();

	FORCEINLINE const bool GetbCombatStance() const { return bCombatStance; }
};
