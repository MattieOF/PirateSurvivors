// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Core/UpgradeType.h"
#include "UObject/Object.h"
#include "WeaponStats.generated.h"

class UWeaponData;

// Represents a player stat upgrade, such as health, damage, etc.
USTRUCT(BlueprintType)
struct FWeaponStatUpgrade
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "GetPropertyNames"))
	FName PropertyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUpgradeType UpgradeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delta;

	FWeaponStatUpgrade()
	{
		PropertyName = NAME_None;
		UpgradeType = EUpgradeType::Add;
		Delta = 0;
	}

	explicit FWeaponStatUpgrade(const FName PropertyName)
	{
		this->PropertyName = PropertyName;
		this->UpgradeType = EUpgradeType::Add;
		this->Delta = 0;
	}
	
	FWeaponStatUpgrade(const FName PropertyName, const EUpgradeType UpgradeType, const float Increase)
	{
		this->PropertyName = PropertyName;
		this->UpgradeType = UpgradeType;
		this->Delta = Increase;
	}
};

/**
 * Stores a weapons stats, such as damage multiplier, etc
 */
UCLASS(EditInlineNew, BlueprintType, Blueprintable, HideCategories = (Object))
class PIRATESURVIVORS_API UWeaponStats : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Damage = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float FireRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float Range = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float ProjectileSpeed = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float Knockback = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float Spread = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float Ammo = 30;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	// int32 MaxAmmo = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float ReloadTime = 2.f;
};