// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UPawnSensingComponent;
class AEnemy;

UCLASS()
class PIRATESURVIVORS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy AI Controller")
	AEnemy* PossessedEnemy = nullptr;
};
