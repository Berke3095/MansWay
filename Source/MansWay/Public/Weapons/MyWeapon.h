#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

class UBoxComponent;
class USphereComponent;
class AMyCharacter;
class AMyWidgetManager;

UCLASS()
class MANSWAY_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
		COMPONENTS
	*/
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponMesh{};

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* WeaponBox{};

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* WeaponSphere{};

private:

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetupComponents();

	FTimerHandle TurnOffPhysicsTimer{};
	void TurnOnPhysics();
	void TurnOffPhysics();
	
	/*
		REFERENCES
	*/
	AMyCharacter* MyCharacter{};
	AMyWidgetManager* WidgetManager{};

public:
	void SetEquippedSettings();
	void SetDroppedSettings();
};
