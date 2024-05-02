// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EnemySpawner.generated.h"

class APiratePlayerCharacter;
struct FEncounterStage;
class UEncounterData;

struct FActiveEncounterStage
{
	float TimeLeft = 0;
	float ClumpTimer = 0;
	FEncounterStage* Stage = nullptr;
};

UCLASS()
class PIRATESURVIVORS_API UEnemySpawner : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Enemy Spawner")
	void SetEncounter(UEncounterData* Encounter);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy Spawner")
	FORCEINLINE int GetAliveEnemies() const { return AliveEnemies; }

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawner")
	void Tick();

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Spawner")
	int AliveEnemies = 0;

private:
	TArray<FActiveEncounterStage> ActiveStages;
	TMap<float, int> EncounterStartTimes;

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawner")
	APiratePlayerCharacter* GetSpawnTarget() const;

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawner")
	bool GetSpawnPoint(APiratePlayerCharacter* Target, FVector& OutSpawnPoint, bool bImportant = false) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy Spawner", meta = (WorldContext = "WorldContextObject"))
	static float GetLowestSquaredDistanceToPlayer(UObject* WorldContextObject, const FVector& Location);
	
	UPROPERTY()
	UEncounterData* CurrentEncounter = nullptr;

	UPROPERTY()
	float CurrentTime = 0;
};
