// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

class UEnemyStats;
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
	float MeleeDistCheckBias = 70.f;

	// If an enemy is a mini-boss, it will have a healthbar
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	bool bIsMiniBoss = false;

	// If an enemy is a boss, it will have a bigger healthbar and will end the level when killed.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	bool bIsBoss = false;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	int XPDropCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data", DisplayName = "Default AI Values")
	TMap<FName, float> DefaultAIValues;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Enemy Data")
	UEnemyStats* Stats = nullptr;
	
	// ----------------- Audio -----------------
	
	// Some enemies are a big deal, and should have a unique sound when they spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data|Sounds")
	USoundBase* SpawnSound = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data|Sounds")
	USoundBase* HitSound = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data|Sounds")
	USoundBase* DeathSound = nullptr;
};
