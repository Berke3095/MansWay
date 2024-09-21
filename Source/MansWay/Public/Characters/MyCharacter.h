#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/MyCharacterEnums.h"
#include "MyCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UCombatComponent;
class USphereComponent;
class UMyAnimInstance;
class UAnimMontage;
class AMyEnemy;


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
	void LockEnemy(float deltaTime);

	/*
		ENUMS
	*/
	ECombatState CombatState{ ECombatState::ECS_NONE };

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

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* AreaTrace{};
	UFUNCTION()
	void OnAreaTraceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnAreaTraceEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void LockOnEnemy();
	AMyEnemy* LockedEnemy{};
	TArray<AMyEnemy*> EnemiesAround{};

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
	void LockLeft();
	void LockRight();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction{};
	
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
