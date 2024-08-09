#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyShield.generated.h"

class UBoxComponent;

UCLASS()
class MANSWAY_API AMyShield : public AActor
{
	GENERATED_BODY()

public:
	AMyShield();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* ShieldMesh{};

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* ShieldBox{};

	void SetupComponents();
};
