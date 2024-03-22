// copyright lololol

#pragma once

#include "CoreMinimal.h"

#define ECC_Player      ECC_GameTraceChannel1
#define ECC_Enemy       ECC_GameTraceChannel2
#define ECC_Projectile  ECC_GameTraceChannel3

/**
 * Module class for pirate survivors 
 */
class PIRATESURVIVORS_API FPirateSurvivorsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
