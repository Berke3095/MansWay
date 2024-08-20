#include "Animations/MyAnimInstance.h"
#include "Characters/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	if (MyCharacter)
	{
		MyCharacterMovement = MyCharacter->GetCharacterMovement();
	}
	else { UE_LOG(LogTemp, Error, TEXT("UMyAnimInstance::NativeInitializeAnimation - MyCharacter is null.")); }
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MyCharacterMovement)
	{
		CharacterDirection = UKismetAnimationLibrary::CalculateDirection(MyCharacterMovement->Velocity, MyCharacter->GetActorRotation());
		Speed = MyCharacter->GetSpeed();

		CharacterYaw = MyCharacter->GetCharacterYaw();
		CharacterPitch = MyCharacter->GetCharacterPitch();

		bSwitchStance = MyCharacter->GetbStanceSwitch();
		if (bSwitchStance)
		{
			RootSpeed = MyCharacter->GetRootSpeed();
			RootDirection = MyCharacter->GetRootDir();
		}
	}
	else { UE_LOG(LogTemp, Error, TEXT("UMyAnimInstance::NativeUpdateAnimation - MyCharacterMovement is null.")); }
}
