// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Core/UpgradeType.h"
#include "UObject/Object.h"
#include "Core/Stats.h"
#include "PlayerStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, float, NewValue);

// Represents a player stat upgrade, such as health, damage, etc.
USTRUCT(BlueprintType)
struct FPlayerStatUpgrade
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "PirateSurvivors.PlayerStats.GetPropertyNames"))
	FName PropertyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUpgradeType UpgradeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delta;

	FPlayerStatUpgrade()
	{
		PropertyName = NAME_None;
		UpgradeType = EUpgradeType::Add;
		Delta = 0;
	}

	explicit FPlayerStatUpgrade(const FName PropertyName)
	{
		this->PropertyName = PropertyName;
		this->UpgradeType = EUpgradeType::Add;
		this->Delta = 0;
	}
	
	FPlayerStatUpgrade(const FName PropertyName, const EUpgradeType UpgradeType, const float Increase)
	{
		this->PropertyName = PropertyName;
		this->UpgradeType = UpgradeType;
		this->Delta = Increase;
	}
};

/**
 * Stores the player's stats, such as health multiplier, damage multipliers, etc.
 */
UCLASS()
class PIRATESURVIVORS_API UPlayerStats : public UObject
{
	GENERATED_BODY()

public:
	// All these properties are declared with the DECLARE_STAT macro from Stats.h
	// They are manually expanded via Rider, as UHT won't recognise UPROPERTIES in macros
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetMaxHealth)
	float MaxHealth = 100;
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChanged OnMaxHealthChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetMaxSpeed)
	float MaxSpeed = 600;
	UFUNCTION(BlueprintCallable)
	void SetMaxSpeed(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChanged OnMaxSpeedChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetArmor)
	float Armor = 0;
	UFUNCTION(BlueprintCallable)
	void SetArmor(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChanged OnArmorChanged;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetLuck)
	float Luck = 0;
	UFUNCTION(BlueprintCallable)
	void SetLuck(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChanged OnLuckChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetUpgradeChoices)
	float UpgradeChoices = 3;
	UFUNCTION(BlueprintCallable)
	void SetUpgradeChoices(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChanged OnUpgradeChoicesChanged;

	// Enemy aggression controls how likely an enemy is to target the player
	// Enemy spawning is based on the amount of enemies targeting each player. Enemies are spawned to target the player with the least enemies targeting them
	// This factor modifies the amount of enemies targeting the player, making the player more or less likely to be targeted
	// For example, when spawning a new enemy:
	//         Player A has 5 enemies targeting them, Player B has 3 enemies targeting them
	//         If both have an aggression factor of 0, the new enemy will target Player B
	//         If Player A has an aggression factor of 2, the new enemy will target a random player
	//         If player A has an aggression factor of 5, the new enemy will target Player A
	//         If Player B has an aggression factor of -3, the new enemy will target Player A
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetEnemyAggressionFactor)
	float EnemyAggressionFactor = 0;
	UFUNCTION(BlueprintCallable)
	void SetEnemyAggressionFactor(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChanged OnEnemyAggressionFactorChanged;
	
	UFUNCTION()
	static TArray<FName> GetPropertyNames();
};
