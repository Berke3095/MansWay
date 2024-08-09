#include "Weapons/MyShield.h"
#include "Components/BoxComponent.h"

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

void AMyShield::SetupComponents()
{
	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield Mesh"));
	if (ShieldMesh)
	{
		RootComponent = ShieldMesh;
		ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ShieldMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - ShieldMesh is null.")); }

	ShieldBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Shield Box"));
	if (ShieldBox)
	{
		ShieldBox->SetupAttachment(ShieldMesh);
		ShieldBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ShieldBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ShieldBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		ShieldBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}
	else { UE_LOG(LogTemp, Error, TEXT("AMyShield::SetupComponents - ShieldBox is null.")); }
}
