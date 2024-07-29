#include "Characters/MyCharacter.h"

// Action library
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Components
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 

// Attributes
#include "GameFramework/CharacterMovementComponent.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupComponents();
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetupReferences();
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UseControllerYaw(DeltaTime);
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
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupReferences - Subsystem is null.")) }
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupReferences - PlayerController is null.")) }
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
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupComponents - CapsuleComponent is null.")) }

	MeshComponent = GetMesh();
	if (MeshComponent)
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupComponents - MeshComponent is null.")) }

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	if (SpringArm)
	{
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->TargetArmLength = StartingTargetArmLength;
		SpringArm->SocketOffset = StartingSocketOffset;
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupComponents - SpringArm is null.")) }

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonShoulderCamera"));
	if (Camera)
	{
		Camera->SetupAttachment(SpringArm);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupComponents - Camera is null.")) }
}

void AMyCharacter::Move(const FInputActionValue& InputValue1)
{
	const FVector2D Value = InputValue1.Get<FVector2D>();

	if (PlayerController)
	{
		if (Value.Y == 1.0f) { ForwardSpeed = 1.0f; }
		else if (Value.Y == -1.0f) { ForwardSpeed = -1.0f; }
		else if (Value.Y == 0.0f) { ForwardSpeed = 0.0f; }

		if (Value.X == 0.0f && Value.Y == 1.0f) { MoveDirection = 2.0f; } // Forward
		else if (Value.X == 0.0f && Value.Y == -1.0f) { MoveDirection = 2.0f; } // Back
		else if ((Value.X == 1.0f && Value.Y == 1.0f) || 
			(Value.X == 1.0f && Value.Y == 0.0f) || 
			(Value.X == 1.0f && Value.Y == -1.0f)) { MoveDirection = 3.0f; } // Right Direction
		else if ((Value.X == -1.0f && Value.Y == -1.0f) || 
			(Value.X == -1.0f && Value.Y == 0.0f) || 
			(Value.X == -1.0f && Value.Y == 1.0f)) { MoveDirection = 1.0f; } // Left Direction
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Move - PlayerController is null.")) }
}

void AMyCharacter::StopMove()
{
	MoveDirection = 2.0f;
	ForwardSpeed = 0.0f;
}

void AMyCharacter::Look(const FInputActionValue& InputValue1)
{
	const FVector2D Value = InputValue1.Get<FVector2D>();

	if (PlayerController)
	{
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::Look - PlayerController is null.")) }
}

void AMyCharacter::UseControllerYaw(float DeltaTime1)
{
	FRotator TargetActorRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	FRotator InterpolatedRotation = FMath::RInterpTo(GetActorRotation(), TargetActorRotation, DeltaTime1, 10.0f);
	SetActorRotation(InterpolatedRotation);
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
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - MoveAction is null.")) }

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - LookAction is null.")) }
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyCharacter::SetupPlayerInputComponent - EnhancedInputComponent is null.")) }
}