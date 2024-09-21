#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

UCLASS()
class MANSWAY_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyAIController();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
