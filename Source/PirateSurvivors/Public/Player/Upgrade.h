// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Upgrade.generated.h"

// Forward decls
class UUpgrade;
class AWeaponFunctionality;
class APiratePlayerState;
class UPlayerStats;
struct FWeaponFunctionalityUpgrade;
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
	Legendary  UMETA(DisplayName = "Legendary"),
	Max		   UMETA(Hidden),
	Debug      UMETA(DisplayName = "Debug")
};

USTRUCT(BlueprintType)
struct FQueuedUpgradeChoice
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Upgrade Choice")
	UWeaponData* Weapon;
	
	UPROPERTY(BlueprintReadWrite, Category = "Upgrade Choice")
	UUpgrade* Upgrade;

	UPROPERTY(BlueprintReadWrite, Category = "Upgrade Choice")
	int TargetWeaponIndex = -1;
};

/**
 * Object containing base information for an upgrade, such as name, description, etc.
 * This class is abstract as it isn't specifically for players or weapons, but contains the data that both of those classes need.
 */
UCLASS(BlueprintType)
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
	TArray<FName> ValidWeaponTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Upgrade")
	TArray<UUpgrade*> Prerequisites;

	// If true, the upgrade will only apply to one weapon. If false, it will apply to all compatible weapons.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Upgrade")
	bool bForOneWeapon = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Upgrade")
	TSubclassOf<UWeaponStats> WeaponStatsClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Upgrade")
	TArray<FWeaponStatUpgrade> StatUpgrades;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Upgrade")
	TArray<FWeaponFunctionalityUpgrade> FunctionalityUpgrades;
	
	UFUNCTION()
	TArray<FName> GetPropertyNames();

	UFUNCTION(BlueprintCallable, Category = "Weapon Upgrade")
	bool VerifyCompatability() const;
	
	UFUNCTION(CallInEditor, Category = "Weapon")
	FORCEINLINE void CheckCompatibility() const { VerifyCompatability(); };

	UFUNCTION(BlueprintCallable, Category = "Weapon Upgrade")
	AWeaponFunctionality* ApplyUpgrade(APiratePlayerState* Player, AWeaponFunctionality* Weapon);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Upgrade")
	bool IsValidForWeapon(AWeaponFunctionality* Weapon) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Upgrade")
	bool IsValidForPlayer(APiratePlayerState* Player) const;
};

UCLASS()
class PIRATESURVIVORS_API UPlayerUpgrade : public UUpgrade
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Upgrade")
	TArray<UUpgrade*> Prerequisites;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Upgrade")
	TArray<FPlayerStatUpgrade> StatUpgrades;

	UFUNCTION(BlueprintCallable, Category = "Player Upgrade")
	void ApplyUpgrade(APiratePlayerState* Player) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player Upgrade")
	bool IsValidForPlayer(APiratePlayerState* Player) const;
};
