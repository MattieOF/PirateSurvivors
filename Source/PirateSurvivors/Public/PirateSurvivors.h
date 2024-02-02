// copyright lololol

#pragma once

#include "CoreMinimal.h"

/**
 * Module class for pirate survivors 
 */
class PIRATESURVIVORS_API FPirateSurvivorsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
