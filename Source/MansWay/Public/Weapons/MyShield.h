#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyShield.generated.h"

class UBoxComponent;
class USphereComponent;
class AMyCharacter;

UCLASS()
class MANSWAY_API AMyShield : public AActor
{
	GENERATED_BODY()

public:
	AMyShield();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*
		COMPONENTS
	*/
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* ShieldMesh{};

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* ShieldBox{};

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* ShieldSphere{};

	void SetupComponents();

	/*
		REFERENCES
	*/
	AMyCharacter* MyCharacter;

public:
	void SetEquippedSettings();
	void SetDroppedSettings();
};
