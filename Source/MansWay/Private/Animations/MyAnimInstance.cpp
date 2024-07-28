#include "Animations/MyAnimInstance.h"

#include "Characters/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	if(!MyCharacter) { UE_LOG(LogTemp, Error, TEXT("UMyAnimInstance::NativeInitializeAnimation - MyCharacter is null.")) }
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MyCharacter)
	{
		CharacterDirection = MyCharacter->GetMoveDirection();
		Speed = MyCharacter->GetSpeed();
	}
	else { UE_LOG(LogTemp, Error, TEXT("UMyAnimInstance::NativeUpdateAnimation - MyCharacter is null.")) }
}
