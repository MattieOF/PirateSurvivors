// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Upgrade.generated.h"

class UWeaponData;
class UWeaponStats;
struct FPlayerStatUpgrade;
struct FWeaponStatUpgrade;

UENUM(BlueprintType)
enum class ERarity : uint8
{
	Common     UMETA(DisplayName = "Common"),
	Uncommon   UMETA(DisplayName = "Uncommon"),
	Rare       UMETA(DisplayName = "Rare"),
	Epic       UMETA(DisplayName = "Epic"),
	Legendary  UMETA(DisplayName = "Legendary")
};

/**
 * Object containing base information for an upgrade, such as name, description, etc.
 * This class is abstract as it isn't specifically for players or weapons, but contains the data that both of those classes need.
 */
UCLASS()
class PIRATESURVIVORS_API UUpgrade : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	ERarity Rarity;
	
	// OLD RARITY LOGIC, keeping it for memories
	// Number from 0 to 1 representing the chance of the upgrade being rolled
	// Used by a weighted random number generator to determine if the upgrade is rolled out of a pool
	// This number also determines the rarity bracket of the upgrade
	// 0 - 0.02: Legendary
	// 0.02 - 0.06: Epic
	// 0.06 - 0.15: Rare
	// 0.15 - 0.4: Uncommon
	// 0.4 - 1: Common
	// Todo: Can we think of better names for these? Like, pirate-themed names?
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	// float Chance = 1;
};

UCLASS()
class PIRATESURVIVORS_API UWeaponUpgrade : public UUpgrade
{
	GENERATED_BODY()

public:
	UWeaponUpgrade();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Upgrade")
	TArray<UWeaponData*> ValidWeapons;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Upgrade")
	TSubclassOf<UWeaponStats> WeaponStatsClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Upgrade")
	TArray<FWeaponStatUpgrade> StatUpgrades;
	
	UFUNCTION()
	TArray<FName> GetPropertyNames();

	UFUNCTION(BlueprintCallable, Category = "Weapon Upgrade")
	bool VerifyCompatability() const;
	
	UFUNCTION(CallInEditor, Category = "Weapon")
	FORCEINLINE void CheckCompatibility() const { VerifyCompatability(); };
};

UCLASS()
class PIRATESURVIVORS_API UPlayerUpgrade : public UUpgrade
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Upgrade")
	TArray<FPlayerStatUpgrade> StatUpgrades;
};
