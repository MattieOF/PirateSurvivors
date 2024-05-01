// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Core/Stats.h"
#include "Core/UpgradeType.h"
#include "UObject/Object.h"
#include "WeaponStats.generated.h"

class AWeaponFunctionality;
class UWeaponData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponStatChanged, float, NewValue);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats", Setter = SetDamage)
	float Damage = 100;
	UFUNCTION(BlueprintCallable)
	void SetDamage(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStatChanged OnDamageChanged;

	// The weapon will fire every FireRateSeconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats", Setter = SetFireRateSeconds)
	float FireRateSeconds = .5f;
	UFUNCTION(BlueprintCallable)
	void SetFireRateSeconds(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStatChanged OnFireRateSecondsChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats", Setter = SetRange)
	float Range = 1000.f;
	UFUNCTION(BlueprintCallable)
	void SetRange(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStatChanged OnRangeChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats", Setter = SetProjectileSpeed)
	float ProjectileSpeed = 1000.f;
	UFUNCTION(BlueprintCallable)
	void SetProjectileSpeed(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStatChanged OnProjectileSpeedChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats", Setter = SetKnockback)
	float Knockback = 0;
	UFUNCTION(BlueprintCallable)
	void SetKnockback(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStatChanged OnKnockbackChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats", Setter = SetSpread)
	float Spread = 0;
	UFUNCTION(BlueprintCallable)
	void SetSpread(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStatChanged OnSpreadChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats", Setter = SetAmmo)
	float Ammo = 30;
	UFUNCTION(BlueprintCallable)
	void SetAmmo(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStatChanged OnAmmoChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats", Setter = SetReloadTime)
	float ReloadTime = 2.f;
	UFUNCTION(BlueprintCallable)
	void SetReloadTime(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStatChanged OnReloadTimeChanged;

	virtual bool IsSupportedForNetworking() const override { return true; };
};
