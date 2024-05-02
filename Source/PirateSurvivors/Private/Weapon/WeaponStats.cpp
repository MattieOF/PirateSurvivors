// copyright lololol

#include "Weapon/WeaponStats.h"

#include "PirateLog.h"
#include "Core/PiratePlayerState.h"
#include "Weapon/WeaponData.h"
#include "Weapon/WeaponFunctionality.h"

DECLARE_PIRATE_STAT_CPP(UWeaponStats, Damage);
DECLARE_PIRATE_STAT_CPP(UWeaponStats, FireRateSeconds);
DECLARE_PIRATE_STAT_CPP(UWeaponStats, Range);
DECLARE_PIRATE_STAT_CPP(UWeaponStats, ProjectileSpeed);
DECLARE_PIRATE_STAT_CPP(UWeaponStats, Knockback);
DECLARE_PIRATE_STAT_CPP(UWeaponStats, Spread);
DECLARE_PIRATE_STAT_CPP(UWeaponStats, Ammo);
DECLARE_PIRATE_STAT_CPP(UWeaponStats, ReloadTime);

bool UWeaponData::VerifyCompatability() const
{
	if (!BaseWeaponStats)
	{
		PIRATE_LOG_ERROR_NOLOC("In %ls, BaseWeaponStats is null! This must be set for the weapon to work.", *GetName());
		return false;
	}

	if (BaseWeaponStats->GetClass() != WeaponStatsSubclass)
	{
		PIRATE_LOG_ERROR_NOLOC("In %ls, BaseWeaponStats is not of the same class as WeaponStatsSubclass!", *GetName());
		return false;
	}

	return true;
}

bool UWeaponData::CanPlayerUseWeapon(APiratePlayerState* Player) const
{
	check(Player);

	auto PlayersWeapons = Player->GetWeapons();
	for (const AWeaponFunctionality* Weapon : *PlayersWeapons)
	{
		if (Weapon && Weapon->GetWeaponData() == this)
			return false;
	}

	// TODO: expand with more features
	
	return true;
}
