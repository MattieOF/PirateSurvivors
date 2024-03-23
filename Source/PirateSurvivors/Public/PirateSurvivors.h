// copyright lololol

#pragma once

#include "CoreMinimal.h"

#define ECC_Player      ECC_GameTraceChannel1
#define ECC_Enemy       ECC_GameTraceChannel2
#define ECC_Projectile  ECC_GameTraceChannel3

DECLARE_STATS_GROUP(TEXT("Pirate Survivors UI"), STATGROUP_PirateUI, STATCAT_Pirate) 

/**
 * Module class for pirate survivors 
 */
class PIRATESURVIVORS_API FPirateSurvivorsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
