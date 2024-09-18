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
class UAnimMontage;


UCLASS()
class MANSWAY_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Movement(float deltaTime);

	/*
		ATTRIBUTES
	*/
	const float DefaultSpeed{ 250.0f };
	const float CombatSpeed{ 125.0 };
	const float SprintSpeed{ 350.0f };

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

	void SwitchStanceCamera(float deltaTime);

	/*
		INPUT
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* CharacterMappingContext{};

	void Move(const FInputActionValue& inputValue);
	void StopMove();
	void Look(const FInputActionValue& inputValue);
	void BasicAttack();
	void Interact();
	void StanceSwitch();
	void Parry();
	void ResetParry();
	void HeavyAttack();

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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* HeavyAction{};
	
	float Speed{};
	void UseControllerYaw(float deltaTime);

	/*
		OFFSET
	*/
	float CharacterYaw{};
	float CharacterPitch{};
	void AimOffset(float deltaTime);

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

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* ParryMontage{};
	bool bCanParry{true};
	FTimerHandle ParryResetTimer{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HeavyMontage{};
	int32 HeavyInc{};

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* LightMontage{};
	int32 LightInc{};

	UFUNCTION()
	void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

private:
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

public:

	FORCEINLINE const float GetCharacterYaw() const { return CharacterYaw; }
	FORCEINLINE const float GetCharacterPitch() const { return CharacterPitch; }
	
	FORCEINLINE const float GetSpeed() const { return Speed; }

	FORCEINLINE void SetClosestInteractable(AActor* closestInteractable) { ClosestInteractable = closestInteractable; }
	FORCEINLINE const AActor* GetClosestInteractable() const { return ClosestInteractable ? ClosestInteractable : nullptr; }

	FORCEINLINE void AddToOverlaps(AActor* interactableToAdd) { if (!Overlaps.Contains(interactableToAdd)) { Overlaps.Add(interactableToAdd); } }
	FORCEINLINE void RemoveFromOverlaps(AActor* interactableToRemove) { if (Overlaps.Contains(interactableToRemove)) { Overlaps.Remove(interactableToRemove); } }
	FORCEINLINE TArray<AActor*> GetOverlappingInteractables() const { return Overlaps; }
	AActor* CalculateClosestOverlap();

	FORCEINLINE const bool GetbCombatStance() const { return bCombatStance; }
};
