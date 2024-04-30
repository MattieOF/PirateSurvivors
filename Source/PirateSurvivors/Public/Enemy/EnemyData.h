﻿// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

class UBehaviorTree;
class UBlackboardData;
/**
 * This class contains the base values of properties all enemies have, such as name, health, damage, etc.
 */
UCLASS()
class PIRATESURVIVORS_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()

public:
	UEnemyData();
	
	// ----------------- Basic Properties -----------------
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	float BaseHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	float BaseDamage = 20;

	// If an enemy is a mini-boss, it will have a healthbar
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	bool bIsMiniBoss = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	USkeletalMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	UBlackboardData* Blackboard = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	TSubclassOf<class AEnemy> EnemySubclass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	TSubclassOf<class AEnemyAIController> EnemyAISubclass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	FVector2D XPDropRange = FVector2D(1.0f, 1.5f);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy Data")
	FORCEINLINE float GetXPDrop() const { return FMath::RandRange(XPDropRange.X, XPDropRange.Y); }

	// ----------------- Audio -----------------
	
	// Some enemies are a big deal, and should have a unique sound when they spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	USoundBase* SpawnSound = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	USoundBase* HitSound = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	USoundBase* DeathSound = nullptr;
};
