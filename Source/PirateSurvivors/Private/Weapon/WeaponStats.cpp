// copyright lololol

#include "Weapon/WeaponStats.h"

#include "PirateLog.h"
#include "Weapon/WeaponData.h"

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
