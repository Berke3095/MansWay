#include "Characters/MyCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CombatComponent.h"
#include "Animations/MyAnimInstance.h"
#include "Animation/AnimMontage.h"


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

	Movement(DeltaTime);
}

void AMyCharacter::Movement(float deltaTime)
{
	AimOffset(deltaTime);
	SwitchStanceCamera(deltaTime);
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
	if (MyAnimInstance)
	{
		MyAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AMyCharacter::OnNotifyBegin);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupReferences - MyAnimInstance is null.")); }
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
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3); // Player 
		MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap); // Enemy weapon
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

void AMyCharacter::SwitchStanceCamera(float deltaTime)
{
	if (bInterpInProcess && SpringArm)
	{
		const FVector CombatStanceOffset = FVector(0.0f, 50.0f, 20.0f);
		FVector SocketToInterp = bCombatStance ? CombatStanceOffset : StartingSocketOffset;

		FVector InterpolatedSocket = FMath::VInterpTo(SpringArm->SocketOffset, SocketToInterp, deltaTime, 2.0);
		SpringArm->SocketOffset = InterpolatedSocket;

		const float CombatTargetArm = 50.0f;
		float TargetArmToInterp = bCombatStance ? CombatTargetArm : StartingTargetArmLength;

		float InterpolatedTargetArm = FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmToInterp, deltaTime, 2.0);
		SpringArm->TargetArmLength = InterpolatedTargetArm;
	}
	else if (!Camera) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::CameraMovement - Camera is null.")); }
}

void AMyCharacter::Move(const FInputActionValue& inputValue)
{
	const FVector2D Value = inputValue.Get<FVector2D>();

	if (PlayerController)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator ControlYawRotation(0, ControlRotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);

		if (bCombatStance) { if (Speed != CombatSpeed) { Speed = CombatSpeed; } }
		else { if (Speed != DefaultSpeed) { Speed = DefaultSpeed; } }
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Move - PlayerController is null.")); }
}

void AMyCharacter::StopMove()
{
	Speed = 0.0f;
}

void AMyCharacter::Look(const FInputActionValue& inputValue)
{
	const FVector2D Value = inputValue.Get<FVector2D>();

	if (PlayerController)
	{
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Look - PlayerController is null.")); }
}

void AMyCharacter::BasicAttack()
{
	if (bCombatStance)
	{
		if (MyAnimInstance && LightMontage)
		{
			if (!MyAnimInstance->Montage_IsPlaying(LightMontage))
			{
				LightInc = 0;
				MyAnimInstance->Montage_Play(LightMontage);
			}
			else { LightInc++; }
		}
		else if (!MyAnimInstance) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::BasicAttack - MyAnimInstance is null.")); }
		else if (!LightMontage) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::BasicAttack - LightMontage is null.")); }
	}
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
	if (CombatComponent->CheckWeapons())
	{
		if (bCombatStance)
		{
			bCombatStance = false;
			GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
			InterpCamera();
		}
		else
		{
			bCombatStance = true;
			GetCharacterMovement()->MaxWalkSpeed = CombatSpeed;
			InterpCamera();
		}
	}
}

void AMyCharacter::Parry()
{
	if (bCanParry && bCombatStance)
	{
		if (MyAnimInstance && ParryMontage)
		{
			bCanParry = false;
			MyAnimInstance->Montage_Play(ParryMontage);

			float ParryResetTime{ 3.0f };
			GetWorldTimerManager().SetTimer(ParryResetTimer, this, &AMyCharacter::ResetParry, ParryResetTime, false);
		}
		else if (!MyAnimInstance) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Parry - MyAnimInstance is null.")); }
		else if (!ParryMontage) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Parry - ParryMontage is null.")); }
	}
}

void AMyCharacter::ResetParry()
{
	if (GetWorldTimerManager().IsTimerActive(ParryResetTimer))
	{
		GetWorldTimerManager().ClearTimer(ParryResetTimer);
	}
	bCanParry = true;

	UE_LOG(LogTemp, Error, TEXT("Timer resetted"));
}

void AMyCharacter::HeavyAttack()
{
	if (bCombatStance)
	{
		if (MyAnimInstance && HeavyMontage)
		{
			if (!MyAnimInstance->Montage_IsPlaying(HeavyMontage))
			{
				HeavyInc = 0;
				MyAnimInstance->Montage_Play(HeavyMontage);
			}
			else { HeavyInc++; }
		}
		else if (!MyAnimInstance) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::HeavyAttack - MyAnimInstance is null.")); }
		else if (!HeavyMontage) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::HeavyAttack - HeavyMontage is null.")); }
	}
}

void AMyCharacter::UseControllerYaw(float deltaTime)
{
	if (CharacterYaw != 0) { CharacterYaw = 0.0f; }

	float InterpTime = bCombatStance ? 10.0f : 3.0f;
	FRotator TargetActorRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	FRotator InterpolatedRotation = FMath::RInterpTo(GetActorRotation(), TargetActorRotation, deltaTime, InterpTime);
	SetActorRotation(InterpolatedRotation);
}

void AMyCharacter::AimOffset(float deltaTime)
{
	FRotator ActorForwardRotation = GetActorForwardVector().Rotation();
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), ActorForwardRotation);
	if (Speed > 0)
	{
		UseControllerYaw(deltaTime);
	}
	else
	{
		if (bCombatStance)
		{
			UseControllerYaw(deltaTime);
		}
		else { CharacterYaw = DeltaRotation.Yaw; }
	}
	CharacterPitch = DeltaRotation.Pitch;
}

void AMyCharacter::InterpCamera()
{
	if (GetWorldTimerManager().IsTimerActive(CameraInterpTimer))
	{
		GetWorldTimerManager().ClearTimer(CameraInterpTimer);
	}

	const float TimeToInterp{ 3.0f };
	GetWorldTimerManager().SetTimer(CameraInterpTimer, this, &AMyCharacter::StopCameraInterp, TimeToInterp, false);
	bInterpInProcess = true;
}

void AMyCharacter::StopCameraInterp()
{
	if (GetWorldTimerManager().IsTimerActive(CameraInterpTimer))
	{
		GetWorldTimerManager().ClearTimer(CameraInterpTimer);
	}
	bInterpInProcess = false;
}

void AMyCharacter::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (MyAnimInstance)
	{
		if (MyAnimInstance->Montage_IsPlaying(HeavyMontage))
		{
			if (NotifyName == "End")
			{
				if (HeavyInc >= 1)
				{
					HeavyInc--;
					FName CurrentSection = MyAnimInstance->Montage_GetCurrentSection(HeavyMontage);
					if (CurrentSection == "Heavy_Slash")
					{
						MyAnimInstance->Montage_Play(HeavyMontage);
						MyAnimInstance->Montage_JumpToSection("Heavy_SpinShield", HeavyMontage);
					}
					else if (CurrentSection == "Heavy_SpinShield") 
					{
						MyAnimInstance->Montage_Play(HeavyMontage);
						MyAnimInstance->Montage_JumpToSection("Heavy_Overhead", HeavyMontage);
					}
				}
				else { MyAnimInstance->Montage_Stop(1.0f, HeavyMontage); }
			}
		}
		else if (MyAnimInstance->Montage_IsPlaying(LightMontage))
		{
			if (NotifyName == "End")
			{
				if (LightInc >= 1)
				{
					LightInc--;
					FName CurrentSection = MyAnimInstance->Montage_GetCurrentSection(LightMontage);
					if (CurrentSection == "Light_Stab")
					{
						MyAnimInstance->Montage_Play(LightMontage);
						MyAnimInstance->Montage_JumpToSection("Light_LeftToRight", LightMontage);
					}
					else if (CurrentSection == "Light_LeftToRight")
					{
						MyAnimInstance->Montage_Play(LightMontage);
						MyAnimInstance->Montage_JumpToSection("Light_Overhead", LightMontage);
					}
				}
				else { MyAnimInstance->Montage_Stop(1.0f, LightMontage); }
			}
		}
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::OnNotifyBegin - MyAnimInstance is null.")); }
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
			EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Started, this, &AMyCharacter::BasicAttack);
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

		if (ParryAction)
		{
			EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &AMyCharacter::Parry);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - ParryAction is null.")); }

		if (HeavyAction)
		{
			EnhancedInputComponent->BindAction(HeavyAction, ETriggerEvent::Started, this, &AMyCharacter::HeavyAttack);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - ParryAction is null.")); }
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
