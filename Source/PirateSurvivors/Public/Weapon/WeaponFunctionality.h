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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponAmmoEmpty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponDataUpdated);

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
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InitialiseLight(APirateSurvivorsCharacter* NewOwner, UWeaponData* Data);

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void OnInitialise();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void OnFire();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FORCEINLINE UWeaponStats* GetWeaponStats() const { return WeaponStats; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FORCEINLINE UWeaponData* GetWeaponData() const { return WeaponData; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	float GetReloadProgress() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	float GetAmmoPercentage() const;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponAmmoEmpty OnWeaponAmmoEmpty;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponDataUpdated OnDataUpdated;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	bool bRequestReposition = false;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool UseAmmo(float Amount);
	
	// Utility functions
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Utility")
	TArray<AActor*> GetAllEnemies() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Utility")
	int GetEnemiesWithinWeaponRange(TArray<AEnemy*>& OutEnemiesInRange);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Utility")
	int GetEnemiesWithinRange(TArray<AEnemy*>& OutEnemiesInRange, float Radius);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Utility")
	int GetEnemiesWithinRangeSorted(TArray<AEnemy*>& OutEnemiesInRange, float Radius);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Utility")
	AEnemy* GetClosestEnemyWithinWeaponRange();

	UFUNCTION(BlueprintCallable, Category = "Weapon Utility")
	AProjectile* SpawnProjectile(const FVector& Position, const FVector& Direction, UProjectileData* ProjectileType) const;

	virtual void PostNetInit() override;
	
	// Internal state
	UPROPERTY(BlueprintReadWrite, Category = "Weapon", Replicated)
	float Ammo = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon", Replicated)
	float ReloadTime = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	float CurrentFireTime = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", Replicated)
	APirateSurvivorsCharacter* OwningCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", Replicated)
	UWeaponData* WeaponData = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UWeaponStats* WeaponStats = nullptr;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
