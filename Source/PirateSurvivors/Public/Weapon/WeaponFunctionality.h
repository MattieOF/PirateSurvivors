// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponFunctionality.generated.h"

class UProjectileData;
class AProjectile;
class AEnemy;
class UWeaponData;
class APirateSurvivorsCharacter;
class UWeaponStats;

UCLASS()
class PIRATESURVIVORS_API AWeaponFunctionality : public AActor
{
	GENERATED_BODY()

public:
	AWeaponFunctionality();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Initialise(APirateSurvivorsCharacter* NewOwner, UWeaponData* Data);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void OnFire();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FORCEINLINE UWeaponStats* GetWeaponStats() const { return WeaponStats; }
	
protected:
	// Utility functions
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Utility")
	TArray<AActor*> GetAllEnemies() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Utility")
	FORCEINLINE TArray<AEnemy*> GetEnemiesWithinWeaponRange() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Utility")
	TArray<AEnemy*> GetEnemiesWithinRange(float Radius) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Utility")
	AEnemy* GetClosestEnemyWithinWeaponRange() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon Utility")
	AProjectile* SpawnProjectile(const FVector& Position, const FVector& Direction, UProjectileData* ProjectileType) const;
	
	// Internal state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	APirateSurvivorsCharacter* OwningCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UWeaponData* WeaponData = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UWeaponStats* WeaponStats = nullptr;
};
