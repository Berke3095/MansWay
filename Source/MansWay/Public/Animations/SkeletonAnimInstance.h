#pragma once

#include "CoreMinimal.h"
#include "Animations/EnemyAnimInstance.h"
#include "SkeletonAnimInstance.generated.h"

UCLASS()
class MANSWAY_API USkeletonAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
	
private:
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaTime) override;
};
