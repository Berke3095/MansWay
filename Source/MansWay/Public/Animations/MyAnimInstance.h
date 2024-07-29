#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

class AMyCharacter;
class UCharacterMovementComponent;

UCLASS()
class MANSWAY_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	AMyCharacter* MyCharacter{};
	UCharacterMovementComponent* MyCharacterMovement{};

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterDirection{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float ForwardSpeed{};
};
