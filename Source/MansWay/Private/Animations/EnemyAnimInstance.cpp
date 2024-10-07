#include "Animations/EnemyAnimInstance.h"
#include "Enemy/MyEnemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Enemy = Cast<AMyEnemy>(TryGetPawnOwner());
	if (!Enemy) { UE_LOG(LogTemp, Error, TEXT("UEnemyAnimInstance::NativeInitializeAnimation - Enemy is null")); }
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!bIsDead)
	{
		if (Enemy)
		{
			EnemySpeed = Enemy->GetEnemySpeed();
			EnemyYaw = Enemy->GetEnemyYaw();
			EnemyPitch = Enemy->GetEnemyPitch();
		}
		else { UE_LOG(LogTemp, Error, TEXT("UEnemyAnimInstance::NativeUpdateAnimation - Enemy is null")) }
	}
}
