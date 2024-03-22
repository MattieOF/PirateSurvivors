// copyright lololol

#include "Player/Upgrade.h"

#include "PirateLog.h"
#include "Weapon/WeaponData.h"
#include "Weapon/WeaponFunctionality.h"
#include "Weapon/WeaponStats.h"

UWeaponUpgrade::UWeaponUpgrade()
{
	WeaponStatsClass = UWeaponStats::StaticClass();
}

TArray<FName> UWeaponUpgrade::GetPropertyNames()
{
	TArray<FName> PropertyNames;
	for (TFieldIterator<FProperty> Prop(WeaponStatsClass ? WeaponStatsClass : UWeaponStats::StaticClass()); Prop; ++Prop) {
		const FProperty* Property = *Prop;
		PropertyNames.Add(Property->GetFName());
	}
	return PropertyNames;
}

bool UWeaponUpgrade::VerifyCompatability() const
{
	if (!WeaponStatsClass)
	{
		PIRATE_LOG_ERROR(FString::Printf(TEXT("Weapon upgrade %ls has no weapon stats class! This must be set for the upgrade to work."), *GetName()));
		return false;
	}
	
	if (ValidWeapons.Num() == 0)
	{
		PIRATE_LOG_ERROR(FString::Printf(TEXT("Weapon upgrade %ls has no valid weapons. This probably isn't intentional!"), *GetName()));
		return false;
	}

	for (const FWeaponStatUpgrade& Upgrade : StatUpgrades)
	{
		if (WeaponStatsClass.Get()->FindPropertyByName(Upgrade.PropertyName) == nullptr)
		{
			PIRATE_LOG_ERROR(FString::Printf(TEXT("Weapon upgrade %ls has an invalid property name: %ls"), *GetName(), *Upgrade.PropertyName.ToString()));
			return false;
		}
	}

	for (const FWeaponFunctionalityUpgrade& Upgrade : FunctionalityUpgrades)
	{
		if (Upgrade.Type == EWeaponFunctionalityUpgradeType::ChangeClass && !Upgrade.ClassValue)
		{
			PIRATE_LOG_ERROR(
				FString::Printf(
					TEXT("Weapon upgrade %ls has a functionality upgrade that changes the class of the weapon, but the class is null!"),
					*GetName()));
			return false;
		}
	}

	for (const UWeaponData* Weapon : ValidWeapons)
	{
		if (!Weapon)
		{
			PIRATE_LOG_ERROR(FString::Printf(TEXT("Weapon upgrade %ls has a null valid weapon!"), *GetName()));
			return false;
		}

		if (!Weapon->WeaponStatsSubclass)
		{
			PIRATE_LOG_ERROR(
				FString::Printf(
					TEXT("Weapon upgrade %ls has weapon %ls set as valid, but the weapon has an unset weapon stats class!"),
					*GetName(), *Weapon->GetName()));
			return false;
		}

		if (!Weapon->WeaponStatsSubclass.Get()->IsChildOf(WeaponStatsClass))
		{
			PIRATE_LOG_ERROR(
				FString::Printf(
					TEXT("Weapon upgrade %ls has weapon %ls set as valid, but it has an incompatible weapon stats class (%s)!"
						" They must either be the same, or the weapons stat class must be a subclass of this upgrades stat class."),
					*GetName(), *Weapon->GetName(), *Weapon->WeaponStatsSubclass.Get()->GetName()));
			return false;
		}

		if (!Weapon->WeaponFunctionalitySubclass)
		{
			PIRATE_LOG_ERROR(
				FString::Printf(
					TEXT("Weapon upgrade %ls has weapon %ls set as valid, but the weapon has an unset weapon functionality class!"),
					*GetName(), *Weapon->GetName()));
			return false;
		}

		for (const FWeaponFunctionalityUpgrade& Upgrade : FunctionalityUpgrades)
		{
			if (Upgrade.Type != EWeaponFunctionalityUpgradeType::ChangeClass)
			{
				if (!Weapon->WeaponFunctionalitySubclass.Get()->FindPropertyByName(Upgrade.PropertyName))
				{
					PIRATE_LOG_ERROR(
						FString::Printf(
							TEXT("Weapon functionality upgrade attempts to modify property %ls, but valid weapon %ls's functionality class does not have this property!"),
							*Upgrade.PropertyName.ToString(), *Weapon->GetName()));
					return false;
				}
			}
		}
	}
	
	return true;
}
