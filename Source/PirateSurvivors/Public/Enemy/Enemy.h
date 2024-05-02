// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Core/PirateSurvivorsCharacter.h"
#include "Enemy.generated.h"

class AEnemyAIController;
class APiratePlayerCharacter;
class UHealthBar;
class UEnemyData;

UCLASS()
class PIRATESURVIVORS_API AEnemy : public APirateSurvivorsCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(float Change, float NewHP);
	
	// Runs on the server when the enemy dies
	UFUNCTION()
	virtual void Die();

	// Runs on both server and clients when the enemy dies
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();
	
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetData(UEnemyData* NewEnemyData, APiratePlayerCharacter* Target = nullptr);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Enemy")
	void Multicast_SetData(UEnemyData* NewEnemyData, APiratePlayerCharacter* Target = nullptr);
	
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetHasHealthBar(bool bHasHealthBar);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy")
	FORCEINLINE UEnemyData* GetData() const { return EnemyData; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bManuallyFaceTarget = false;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetManuallyFaceTarget(bool bNewValue);
	
protected:
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UEnemyData* EnemyData = nullptr;

	// Usually null, only valid if the enemy is a mini-boss
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHealthBar* HealthBar = nullptr;

	UPROPERTY()
	AEnemyAIController* EnemyAIController = nullptr;

	bool bHasSetData = false;
};
