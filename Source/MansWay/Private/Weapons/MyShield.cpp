#include "Weapons/MyShield.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/MyCharacter.h"

AMyShield::AMyShield()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupComponents();
}

void AMyShield::BeginPlay()
{
	Super::BeginPlay();

}

void AMyShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyShield::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMyCharacter>())
	{
		MyCharacter = Cast<AMyCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->SetOverlappingInteractable(this);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyShield::OnSphereOverlap - MyCharacter is null.")); }

		UE_LOG(LogTemp, Warning, TEXT("Overlapping with MyCharacter."));
	}
}

void AMyShield::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MyCharacter)
	{
		if (MyCharacter->GetMyOverlappingInteractable() == this)
		{
			MyCharacter->SetOverlappingInteractable(nullptr);
			UE_LOG(LogTemp, Warning, TEXT("Stopped overlapping"));
		}
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::OnSphereEndOverlap - MyCharacter is null.")); }
}

void AMyShield::SetupComponents()
{
	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield Mesh"));
	if (ShieldMesh)
	{
		RootComponent = ShieldMesh;
		ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ShieldMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Shield type
		ShieldMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - ShieldMesh is null.")); }

	ShieldBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Shield Box"));
	if (ShieldBox)
	{
		ShieldBox->SetupAttachment(ShieldMesh);
		ShieldBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ShieldBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Shield type
		ShieldBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ShieldBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		ShieldBox->OnComponentBeginOverlap.AddDynamic(this, &AMyShield::OnSphereOverlap);
		ShieldBox->OnComponentEndOverlap.AddDynamic(this, &AMyShield::OnSphereEndOverlap);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - ShieldBox is null.")); }

	ShieldSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Shield Sphere"));
	if (ShieldSphere)
	{
		ShieldSphere->SetupAttachment(ShieldMesh);
		ShieldSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ShieldSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Shield type
		ShieldSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ShieldSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		ShieldSphere->OnComponentBeginOverlap.AddDynamic(this, &AMyShield::OnSphereOverlap);
		ShieldSphere->OnComponentEndOverlap.AddDynamic(this, &AMyShield::OnSphereEndOverlap);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - ShieldSphere is null.")); }
}

void AMyShield::SetEquippedSettings()
{
	if (GetOwner()->IsA<AMyCharacter>())
	{
		if (ShieldSphere)
		{
			ShieldSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ShieldSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetEquippedSettings - ShieldSphere is null.")); }

		if (ShieldBox)
		{
			ShieldBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
			ShieldBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Overlap); // Can overlap enemy
		}
		else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetEquippedSettings - ShieldBox is null.")); }
	}
}

void AMyShield::SetDroppedSettings()
{
	if (ShieldSphere)
	{
		ShieldSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ShieldSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetDroppedSettings - ShieldSphere is null.")); }

	if (ShieldBox)
	{
		ShieldBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ShieldBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetDroppedSettings - ShieldBox is null.")); }
}
