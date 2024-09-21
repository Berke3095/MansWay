#pragma once

#include "CoreMinimal.h"
#include "Animations/EnemyAnimInstance.h"
#include "SkeletonAnimInstance.generated.h"

UCLASS()
class MANSWAY_API USkeletonAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()
	
private:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
};
