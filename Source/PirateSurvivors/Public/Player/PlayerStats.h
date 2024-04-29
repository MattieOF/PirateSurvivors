// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Core/Stats.h"
#include "Core/UpgradeType.h"
#include "UObject/Object.h"
#include "PlayerStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChanged, float, NewValue);

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
	FOnStatChanged OnMaxHealthChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetMaxSpeed)
	float MaxSpeed = 1000;
	UFUNCTION(BlueprintCallable)
	void SetMaxSpeed(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnMaxSpeedChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetArmor)
	float Armor = 0;
	UFUNCTION(BlueprintCallable)
	void SetArmor(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnArmorChanged;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetLuck)
	float Luck = 0;
	UFUNCTION(BlueprintCallable)
	void SetLuck(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnLuckChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetUpgradeChoices)
	float UpgradeChoices = 8;
	UFUNCTION(BlueprintCallable)
	void SetUpgradeChoices(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnUpgradeChoicesChanged;

	UFUNCTION()
	static TArray<FName> GetPropertyNames();
};
