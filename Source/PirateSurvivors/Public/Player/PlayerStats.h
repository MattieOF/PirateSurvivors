﻿// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerStats.generated.h"

// Used to determine how the stat should be upgraded
UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	Add       UMETA(DisplayName = "Add"),
	Multiply  UMETA(DisplayName = "Multiply")
};

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float MaxHealth = 100;

	UFUNCTION()
	static TArray<FName> GetPropertyNames();
};
