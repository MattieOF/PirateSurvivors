// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Core/UpgradeType.h"
#include "UObject/Object.h"
#include "WeaponStats.generated.h"

class AWeaponFunctionality;
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

UENUM(BlueprintType)
enum class EWeaponFunctionalityUpgradeType : uint8
{
	SetBool,
	SetFloat,
	AddFloat,
	ChangeClass,
	CallFunction
};

USTRUCT(BlueprintType)
struct FWeaponFunctionalityUpgrade
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PropertyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponFunctionalityUpgradeType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EWeaponFunctionalityUpgradeType::SetBool", EditConditionHides))
	bool bBoolValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EWeaponFunctionalityUpgradeType::SetFloat || Type == EWeaponFunctionalityUpgradeType::AddFloat", EditConditionHides))
	float FloatValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EWeaponFunctionalityUpgradeType::ChangeClass", EditConditionHides))
	TSubclassOf<AWeaponFunctionality> ClassValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Type == EWeaponFunctionalityUpgradeType::CallFunction", EditConditionHides))
	FName FunctionToCall;

	FWeaponFunctionalityUpgrade()
	{
		PropertyName = NAME_None;
		Type = EWeaponFunctionalityUpgradeType::SetBool;
		bBoolValue = false;
		FloatValue = 0;
		ClassValue = nullptr;
	}

	explicit FWeaponFunctionalityUpgrade(const FName FlagName)
	{
		this->PropertyName = FlagName;
		Type = EWeaponFunctionalityUpgradeType::SetBool;
		bBoolValue = false;
		FloatValue = 0;
		ClassValue = nullptr;
	}
	
	FWeaponFunctionalityUpgrade(const FName FlagName, const bool Value)
	{
		this->PropertyName = FlagName;
		Type = EWeaponFunctionalityUpgradeType::SetBool;
		bBoolValue = Value;
		FloatValue = 0;
		ClassValue = nullptr;
	}

	FWeaponFunctionalityUpgrade(const FName FlagName, const float Value)
	{
		this->PropertyName = FlagName;
		Type = EWeaponFunctionalityUpgradeType::SetFloat;
		bBoolValue = false;
		FloatValue = Value;
		ClassValue = nullptr;
	}

	FWeaponFunctionalityUpgrade(const FName FlagName, TSubclassOf<AWeaponFunctionality> Value)
	{
		this->PropertyName = FlagName;
		Type = EWeaponFunctionalityUpgradeType::ChangeClass;
		bBoolValue = false;
		FloatValue = 0;
		ClassValue = Value;
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

	// The weapon will fire every FireRateSeconds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float FireRateSeconds = .5f;

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

	virtual bool IsSupportedForNetworking() const override { return true; };
};
