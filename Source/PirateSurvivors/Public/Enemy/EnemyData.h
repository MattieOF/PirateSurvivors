// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

/**
 * This class contains the base values of properties all enemies have, such as name, health, damage, etc.
 */
UCLASS()
class PIRATESURVIVORS_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()

public:
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
	USkeletalMesh* Mesh;
};
