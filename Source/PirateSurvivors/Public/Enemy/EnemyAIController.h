// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

// Forward decls
class APirateSurvivorsCharacter;
class UPawnSensingComponent;
class AEnemy;

UCLASS()
class PIRATESURVIVORS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	UFUNCTION(BlueprintCallable, Category = "Enemy AI Controller")
	void SetTarget(APirateSurvivorsCharacter* NewTarget);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy AI Controller")
	FORCEINLINE bool IsTargetPlayer() const { return bIsTargetPlayer; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy AI Controller")
	FORCEINLINE APirateSurvivorsCharacter* GetTarget() const { return Target; }

	UFUNCTION(BlueprintCallable, Category = "Enemy AI Controller")
	void SetIsMeleeAttacking(bool bNewValue);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy AI Controller")
	FORCEINLINE bool IsMeleeAttacking() const { return bIsMeleeAttacking; }

	UFUNCTION(BlueprintCallable, Category = "Enemy AI Controller")
	void PerformMeleeAttack() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy AI Controller")
	FName TargetPropertyName = "Target";
	
	UPROPERTY(BlueprintReadOnly, Category = "Enemy AI Controller")
	AEnemy* PossessedEnemy = nullptr;

	UPROPERTY()
	APirateSurvivorsCharacter* Target = nullptr;

	UFUNCTION()
	void OnPossessedEnemyDeath();
	
	bool bIsTargetPlayer = false;
	bool bIsMeleeAttacking = false;
};
