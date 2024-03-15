// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileData.generated.h"

/**
 * Data relating to a projectile type
 */
UCLASS()
class PIRATESURVIVORS_API UProjectileData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Data")
	FText Name = FText::FromString("Projectile");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Data")
	FVector2D DamageMultiplier = FVector2D(1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Data")
	FVector2D SpeedMultiplier = FVector2D(1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Data")
	float LifeTime = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Data")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Data")
	TSubclassOf<class AProjectile> ProjectileSubclass;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Projectile Data")
	FORCEINLINE float GetRandomSpeedMultiplier() const
	{
		return FMath::RandRange(SpeedMultiplier.X, SpeedMultiplier.Y);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Projectile Data")
	FORCEINLINE float GetRandomDamageMultiplier() const
	{
		return FMath::RandRange(DamageMultiplier.X, DamageMultiplier.Y);
	}
};
