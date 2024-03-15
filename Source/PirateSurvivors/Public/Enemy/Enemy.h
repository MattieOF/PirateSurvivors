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

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetData(UEnemyData* NewEnemyData);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Enemy")
	void Multicast_SetData(UEnemyData* NewEnemyData);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UEnemyData* EnemyData = nullptr;

	// Usually null, only valid if the enemy is a mini-boss
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UUserWidget* HealthBar = nullptr;
};
