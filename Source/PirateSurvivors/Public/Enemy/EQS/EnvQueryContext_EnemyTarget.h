// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_EnemyTarget.generated.h"

UCLASS()
class PIRATESURVIVORS_API UEnvQueryContext_EnemyTarget : public UEnvQueryContext
{
	GENERATED_BODY()

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
