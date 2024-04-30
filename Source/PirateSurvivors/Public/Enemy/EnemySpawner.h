// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EnemySpawner.generated.h"

class UEncounterData;

UCLASS()
class PIRATESURVIVORS_API UEnemySpawner : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Enemy Spawner")
	void SetEncounter(UEncounterData* Encounter);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy Spawner")
	FORCEINLINE int GetAliveEnemies() const { return AliveEnemies; }
	
private:
	int AliveEnemies = 0;
};
