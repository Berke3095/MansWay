#include "Characters/MyCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CombatComponent.h"
#include "Animation/AnimMontage.h"
#include "Animations/MyAnimInstance.h"


AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupComponents();
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupReferences();
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);
}

void AMyCharacter::SetupReferences()
{
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupReferences - Subsystem is null.")); }
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupReferences - PlayerController is null.")); }

	MyAnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (!MyAnimInstance) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupReferences - MyAnimInstance is null.")); }
}

void AMyCharacter::SetupComponents()
{
	CapsuleComponent = GetCapsuleComponent();
	if (CapsuleComponent)
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap); // Weapon
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupComponents - CapsuleComponent is null.")); }

	MeshComponent = GetMesh();
	if (MeshComponent)
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3); // Player 
		MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap); // Weapon
		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block); // PhysicsTool
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupComponents - MeshComponent is null.")); }

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (SpringArm)
	{
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->TargetArmLength = StartingTargetArmLength;
		SpringArm->SocketOffset = StartingSocketOffset;
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupComponents - SpringArm is null.")); }

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonShoulder Camera"));
	if (Camera)
	{
		Camera->SetupAttachment(SpringArm);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupComponents - Camera is null.")); }

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	if (!CombatComponent) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupComponents - CombatComponent is null.")); }
}

void AMyCharacter::Move(const FInputActionValue& InputValue1)
{
	const FVector2D Value = InputValue1.Get<FVector2D>();

	if (PlayerController)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator ControlYawRotation(0, ControlRotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);

		if (Speed != DefaultSpeed) { Speed = DefaultSpeed; }

		if (Value.Y == 1.0f)
		{
			if (Value.X == 1.0f) { if (CombatLocomoInt != 2) { CombatLocomoInt = 2; } } // Right_Forward
			else if (Value.X == 0.0f) { if (CombatLocomoInt != 1) { CombatLocomoInt = 1; } } // Forward
			else if (Value.X == -1.0f) { if (CombatLocomoInt != 8) { CombatLocomoInt = 8; } } // Left_Forward
		}
		else if (Value.Y == 0.0f)
		{
			if (Value.X == 1.0f) { if (CombatLocomoInt != 3) { CombatLocomoInt = 3; } } // Right
			else if (Value.X == 0.0f) { if (CombatLocomoInt != 0) { CombatLocomoInt = 0; } } // Idle
			else if (Value.X == -1.0f) { if (CombatLocomoInt != 7) { CombatLocomoInt = 7; } } // Left
		}
		else if (Value.Y == -1.0f)
		{
			if (Value.X == 1.0f) { if (CombatLocomoInt != 4) { CombatLocomoInt = 4; } } // Right_Backward
			else if (Value.X == 0.0f) { if (CombatLocomoInt != 5) { CombatLocomoInt = 5; } } // Backward
			else if (Value.X == -1.0f) { if (CombatLocomoInt != 6) { CombatLocomoInt = 6; } } // Left_Backward
		}

		if (MyAnimInstance)
		{
			if (bStanceSwitch)
			{
				if (CombatLocomoMontage && !MyAnimInstance->Montage_IsPlaying(CombatLocomoMontage))
				{
					FName SectionName{};
					switch (CombatLocomoInt)
					{
					case 0: // Idle
						SectionName = FName("Idle");
						break;
					case 1: // Forward
						SectionName = FName("Forward");
						break;
					case 2: // Right_Forward
						SectionName = FName("Right_Forward");
						break;
					case 3: // Right
						SectionName = FName("Right");
						break;
					case 4: // Right_Backward
						SectionName = FName("Right_Backward");
						break;
					case 5: // Backward
						SectionName = FName("Backward");
						break;
					case 6: // Left_Backward
						SectionName = FName("Left_Backward");
						break;
					case 7: // Left
						SectionName = FName("Left");
						break;
					case 8: // Left_Forward
						SectionName = FName("Left_Forward");
						break;
					default:
						break;
					}

					MyAnimInstance->Montage_Play(CombatLocomoMontage);
					MyAnimInstance->Montage_JumpToSection(SectionName, CombatLocomoMontage);
				}
				else if (!MyAnimInstance) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Move - MyAnimInstance is null.")); }
				else if (!CombatLocomoMontage) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Move - CombatLocomoMontage is null.")); }
			}
			else
			{
				if (CombatLocomoMontage && MyAnimInstance->Montage_IsPlaying(CombatLocomoMontage))
				{
					MyAnimInstance->Montage_Stop(0.5f, CombatLocomoMontage);
				}
				else if (!MyAnimInstance) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Move - MyAnimInstance is null.")); }
				else if (!CombatLocomoMontage) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Move - CombatLocomoMontage is null.")); }
			}
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Move - MyAnimInstance is null.")); }
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Move - PlayerController is null.")); }
}

void AMyCharacter::StopMove()
{
	Speed = 0.0f;

	if (!bStanceSwitch)
	{
		if (CombatLocomoMontage && MyAnimInstance->Montage_IsPlaying(CombatLocomoMontage))
		{
			MyAnimInstance->Montage_Stop(0.5f, CombatLocomoMontage);
		}
		else if (!MyAnimInstance) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::StopMove - MyAnimInstance is null.")); }
		else if (!CombatLocomoMontage) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::StopMove - CombatLocomoMontage is null.")); }
	}
}

void AMyCharacter::Look(const FInputActionValue& InputValue1)
{
	const FVector2D Value = InputValue1.Get<FVector2D>();

	if (PlayerController)
	{
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Look - PlayerController is null.")); }
}

void AMyCharacter::BasicAttack()
{

}

void AMyCharacter::Interact()
{
	if (CombatComponent && ClosestInteractable)
	{
		CombatComponent->EquipInteractable(ClosestInteractable);
	}
	else if (!CombatComponent) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Interact - CombatComponent is null.")); }
}

void AMyCharacter::StanceSwitch()
{
	if (bStanceSwitch)
	{
		bStanceSwitch = false;
	}
	else { bStanceSwitch = true; }
}

void AMyCharacter::UseControllerYaw(float DeltaTime1)
{
	if (CharacterYaw != 0) { CharacterYaw = 0.0f; }

	FRotator TargetActorRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	FRotator InterpolatedRotation = FMath::RInterpTo(GetActorRotation(), TargetActorRotation, DeltaTime1, 3.0f);
	SetActorRotation(InterpolatedRotation);
}

void AMyCharacter::AimOffset(float DeltaTime1)
{
	FRotator ActorForwardRotation = GetActorForwardVector().Rotation();
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), ActorForwardRotation);
	if (Speed > 0)
	{
		UseControllerYaw(DeltaTime1);
	}
	else
	{
		CharacterYaw = DeltaRotation.Yaw;
	}
	CharacterPitch = DeltaRotation.Pitch;
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyCharacter::StopMove);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - MoveAction is null.")); }

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - LookAction is null.")); }

		if (BasicAttackAction)
		{
			EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::BasicAttack);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - BasicAttackAction is null.")); }

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMyCharacter::Interact);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - InteractAction is null.")); }

		if (StanceSwitchAction)
		{
			EnhancedInputComponent->BindAction(StanceSwitchAction, ETriggerEvent::Started, this, &AMyCharacter::StanceSwitch);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - StanceSwitchAction is null.")); }
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - EnhancedInputComponent is null.")); }
}

AActor* AMyCharacter::CalculateClosestOverlap()
{
	AActor* ClosestActor = nullptr;
	float MinDistance = FLT_MAX;
	for (AActor* OverlappedActor : Overlaps)
	{
		float Distance = FVector::Dist(GetActorLocation(), OverlappedActor->GetActorLocation());

		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestActor = OverlappedActor;
		}
	}

	return ClosestActor;
}
