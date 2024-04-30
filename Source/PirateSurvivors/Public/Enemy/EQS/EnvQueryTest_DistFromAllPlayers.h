// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_DistFromAllPlayers.generated.h"

UCLASS()
class PIRATESURVIVORS_API UEnvQueryTest_DistFromAllPlayers : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()
	
	/** context */
	UPROPERTY(EditDefaultsOnly, Category=Distance)
	TSubclassOf<UEnvQueryContext> DistanceTo;
	
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
