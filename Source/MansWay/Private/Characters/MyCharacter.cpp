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
#include "Components/SphereComponent.h"
#include "Enemy/MyEnemy.h"


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
	LockEnemy(DeltaTime);
}

void AMyCharacter::Movement(float deltaTime)
{
	AimOffset(deltaTime);
	SwitchStanceCamera(deltaTime);
}

void AMyCharacter::LockEnemy(float deltaTime)
{
	if (LockedEnemy)
	{
		FVector enemyLocation = LockedEnemy->GetActorLocation();
		FVector actorLoc = GetActorLocation();
		FVector dirToEnemy = (enemyLocation - actorLoc).GetSafeNormal();

		FRotator targetRot = dirToEnemy.Rotation();

		if (PlayerController)
		{
			targetRot.Yaw -= 20.0f;
			FRotator newRot = FMath::RInterpTo(PlayerController->GetControlRotation(), targetRot, deltaTime, 5.0f);
			PlayerController->SetControlRotation(newRot);
		}
	}
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
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Overlap); // NeutralItem
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupComponents - CapsuleComponent is null.")); }

	MeshComponent = GetMesh();
	if (MeshComponent)
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3); // Player 
		MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap); // Enemy weapon
		MeshComponent->bUseAttachParentBound = true;
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

	AreaTrace = CreateDefaultSubobject<USphereComponent>(TEXT("AreaTrace"));
	if(AreaTrace)
	{
		AreaTrace->SetupAttachment(RootComponent);
		AreaTrace->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AreaTrace->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5); // EnemyTargeter
		AreaTrace->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaTrace->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // Enemy
		AreaTrace->bUseAttachParentBound = true;
		AreaTrace->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnAreaTraceOverlap);
		AreaTrace->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnAreaTraceEndOverlap);
	}
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

void AMyCharacter::OnAreaTraceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMyEnemy>())
	{
		AMyEnemy* enemy = Cast<AMyEnemy>(OtherActor);
		EnemiesAround.AddUnique(enemy);
		if (bCombatStance) { LockOnEnemy(); }
	}
}

void AMyCharacter::OnAreaTraceEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AMyEnemy>())
	{
		AMyEnemy* enemy = Cast<AMyEnemy>(OtherActor);
		if (EnemiesAround.Contains(enemy))
		{
			EnemiesAround.Remove(enemy);
			if (EnemiesAround.Num() == 0) 
			{ 
				LockedEnemy = nullptr; 
			}
		}
	}
}

void AMyCharacter::LockOnEnemy()
{
	AMyEnemy* ClosestEnemy = nullptr;
	float MinDistance = FLT_MAX;
	for (AMyEnemy* Enemy : EnemiesAround)
	{
		float Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());

		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestEnemy = Enemy;
		}
	}

	LockedEnemy = ClosestEnemy;
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
	if (!LockedEnemy)
	{
		const FVector2D Value = inputValue.Get<FVector2D>();

		if (PlayerController)
		{
			AddControllerYawInput(Value.X);
			AddControllerPitchInput(Value.Y);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Look - PlayerController is null.")); }
	}
}

void AMyCharacter::BasicAttack()
{
	if (bCombatStance && (CombatState == ECombatState::ECS_NONE || CombatState == ECombatState::ECS_BasicAttacking))
	{
		if (MyAnimInstance && LightMontage)
		{
			if (!MyAnimInstance->Montage_IsPlaying(LightMontage))
			{
				LightInc = 0;
				MyAnimInstance->Montage_Play(LightMontage);
				CombatState = ECombatState::ECS_BasicAttacking;
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
		CombatComponent->EquipInteractable(this, ClosestInteractable);
	}
	else if (!CombatComponent) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Interact - CombatComponent is null.")); }
}

void AMyCharacter::StanceSwitch()
{
	if (CombatComponent->CheckWeapons() && CombatState == ECombatState::ECS_NONE)
	{
		if (bCombatStance)
		{
			bCombatStance = false;
			GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
			InterpCamera();
			if (LockedEnemy) { LockedEnemy = nullptr; }
		}
		else
		{
			bCombatStance = true;
			GetCharacterMovement()->MaxWalkSpeed = CombatSpeed;
			InterpCamera();
			if (EnemiesAround.Num() > 0) { LockOnEnemy(); }
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
			CombatState = ECombatState::ECS_Parrying;

			float parryResetTime{ 3.0f };
			GetWorldTimerManager().SetTimer(ParryResetTimer, this, &AMyCharacter::ResetParry, parryResetTime, false);
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
}

void AMyCharacter::HeavyAttack()
{
	if (bCombatStance && (CombatState == ECombatState::ECS_NONE || CombatState == ECombatState::ECS_HeavyAttacking))
	{
		if (MyAnimInstance && HeavyMontage)
		{
			if (!MyAnimInstance->Montage_IsPlaying(HeavyMontage))
			{
				HeavyInc = 0;
				MyAnimInstance->Montage_Play(HeavyMontage);
				CombatState = ECombatState::ECS_HeavyAttacking;
			}
			else { HeavyInc++; }
		}
		else if (!MyAnimInstance) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::HeavyAttack - MyAnimInstance is null.")); }
		else if (!HeavyMontage) { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::HeavyAttack - HeavyMontage is null.")); }
	}
}

void AMyCharacter::LockLeft()
{

}

void AMyCharacter::LockRight()
{

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
				else 
				{ 
					MyAnimInstance->Montage_Stop(1.0f, HeavyMontage); 
					CombatState = ECombatState::ECS_NONE;
				}
			}
			else if (NotifyName == "Reset") 
			{ 
				CombatState = ECombatState::ECS_NONE; 
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
				else 
				{ 
					MyAnimInstance->Montage_Stop(0.5f, LightMontage);
					CombatState = ECombatState::ECS_NONE;
				}
			}
			else if (NotifyName == "Reset") 
			{ 
				CombatState = ECombatState::ECS_NONE; 
			}
		}
		else if (MyAnimInstance->Montage_IsPlaying(ParryMontage))
		{
			if (NotifyName == "Reset") 
			{ 
				CombatState = ECombatState::ECS_NONE; 
			}
		}
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::OnNotifyBegin - MyAnimInstance is null.")); }
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AMyCharacter::BasicAttack);
	PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &AMyCharacter::HeavyAttack);
	PlayerInputComponent->BindKey(EKeys::F, IE_Pressed, this, &AMyCharacter::Interact);
	PlayerInputComponent->BindKey(EKeys::LeftControl, IE_Pressed, this, &AMyCharacter::StanceSwitch);
	PlayerInputComponent->BindKey(EKeys::LeftAlt, IE_Pressed, this, &AMyCharacter::Parry);
	PlayerInputComponent->BindKey(EKeys::Q, IE_Pressed, this, &AMyCharacter::LockLeft);
	PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &AMyCharacter::LockRight);

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
