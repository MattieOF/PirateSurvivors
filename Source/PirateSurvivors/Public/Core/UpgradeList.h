// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UpgradeList.generated.h"

class UWeaponData;
// Forward decls
struct FQueuedUpgradeChoice;
class UUpgrade;
class APiratePlayerState;
class UPlayerUpgrade;
class UWeaponUpgrade;
enum class ERarity : uint8;

/**
 * A utility class to hold all the upgrades in the game, and provide functions to get
 * upgrades for players and weapons, taking into account their luck and current inventory.
 */
UCLASS()
class PIRATESURVIVORS_API UUpgradeList : public UObject
{
	GENERATED_BODY()

public:
	UUpgradeList();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upgrade List")
	TArray<UWeaponUpgrade*> GetAllWeaponUpgrades();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upgrade List")
	TArray<UWeaponUpgrade*> GetAllWeaponUpgradesOfRarity(ERarity Rarity) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upgrade List")
	TArray<UPlayerUpgrade*> GetAllPlayerUpgrades();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upgrade List")
	TArray<UPlayerUpgrade*> GetAllPlayerUpgradesOfRarity(ERarity Rarity) const;

	UFUNCTION(BlueprintCallable, Category = "Upgrade List")
	TArray<FQueuedUpgradeChoice> GetPlayerUpgradeChoices(APiratePlayerState* Player) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPlayerUpgrade* GetPlayerPlayerUpgradeChoice(APiratePlayerState* Player, const TArray<UUpgrade*>& Blacklist) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UWeaponUpgrade* GetPlayerWeaponUpgradeChoice(APiratePlayerState* Player, const TArray<UUpgrade*>& Blacklist) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Upgrade List")
	TArray<FQueuedUpgradeChoice> GetWeaponChoices(APiratePlayerState* Player);
	
protected:
	TMap<ERarity, TArray<UWeaponUpgrade*>> WeaponUpgrades;
	UPROPERTY(BlueprintReadOnly, Category = "Upgrade List")
	int TotalWeaponUpgrades = 0;
	TMap<ERarity, TArray<UPlayerUpgrade*>> PlayerUpgrades;
	UPROPERTY(BlueprintReadOnly, Category = "Upgrade List")
	int TotalPlayerUpgrades = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Upgrade List")
	int TotalWeapons = 0;

	UPROPERTY()
	TArray<UWeaponData*> Weapons;
};
