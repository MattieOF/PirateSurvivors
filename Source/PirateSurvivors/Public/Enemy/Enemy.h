// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Core/PirateSurvivorsCharacter.h"
#include "Enemy.generated.h"

class UEnemyData;

UCLASS()
class PIRATESURVIVORS_API AEnemy : public APirateSurvivorsCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void BeginPlay() override;

protected:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetData(UEnemyData* NewEnemyData);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UEnemyData* EnemyData;
};
