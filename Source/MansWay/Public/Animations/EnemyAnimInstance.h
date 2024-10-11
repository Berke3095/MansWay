#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/MyEnemyEnums.h"
#include "EnemyAnimInstance.generated.h"

class AMyEnemy;

UCLASS()
class MANSWAY_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float EnemyPitch{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float EnemyYaw{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float EnemySpeed{};

	AMyEnemy* Enemy{};

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState{ EEnemyState::ES_NONE };

public:
	FORCEINLINE void SetEnemyState(EEnemyState enemyState) { if (EnemyState != enemyState) { EnemyState = enemyState; } }
};
