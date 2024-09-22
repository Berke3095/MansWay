#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

class AMyCharacter;
class UCharacterMovementComponent;

UCLASS(Abstract)
class MANSWAY_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual ~UMyAnimInstance() {}

private:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	AMyCharacter* MyCharacter{};
	UCharacterMovementComponent* MyCharacterMovement{};

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterDirection{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterPitch{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterYaw{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bCombatStance{};
};
