#pragma once

UENUM(BlueprintType)
enum class EEnemyCombatState : uint8
{
	EECS_Attacking UMETA(DisplayName = "Attacking"),
	EECS_NONE UMETA(DisplayName = "NONE")
};
