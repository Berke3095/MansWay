#pragma once

UENUM(BlueprintType)
enum class EEnemyCombatState : uint8
{
	EECS_Attacking UMETA(DisplayName = "Attacking"),
	EECS_NONE UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	ES_Dead UMETA(DisplayName = "Dead"),
	ES_Stunned UMETA(DisplayName = "Stunned"),
	ES_NONE UMETA(DisplayName = "NONE")
};
