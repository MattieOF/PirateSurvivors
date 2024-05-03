// copyright lololol

#include "Enemy/EnemySpawner.h"

#include "NavigationSystem.h"
#include "PirateLog.h"
#include "AI/NavigationSystemBase.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerState.h"
#include "Enemy/EncounterData.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyData.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/PlayerController.h"

class UNavigationSystemV1;

void UEnemySpawner::SetEncounter(UEncounterData* Encounter)
{
	CurrentEncounter = Encounter;
	CurrentTime = 0;

	SpawnPoints.Empty();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "SpawnPoint", SpawnPoints);

	// Build the encounter start times map
	EncounterStartTimes.Empty();
	for (int i = 0; i < Encounter->Stages.Num(); i++)
	{
#if WITH_EDITOR
		if (EncounterStartTimes.Contains(Encounter->Stages[i].StartTimeSeconds))
			PIRATE_LOGC_ERROR(GetWorld(), "Encounter has multiple stages starting at the same time (%f)! This is not supported!", Encounter->Stages[i].StartTimeSeconds);
#endif
		EncounterStartTimes.Add(Encounter->Stages[i].StartTimeSeconds, i);
	}
	EncounterStartTimes.KeySort([](const float A, const float B) { return A < B; });
}

void UEnemySpawner::Tick()
{
	float Delta = GetWorld()->GetDeltaSeconds();
	CurrentTime += Delta;

	// Check if we need to start a stage
	for (const auto& StageTime : EncounterStartTimes)
	{
		if (CurrentTime >= StageTime.Key)
		{
			ActiveStages.Add({CurrentEncounter->Stages[StageTime.Value].LengthSeconds, 0, &CurrentEncounter->Stages[StageTime.Value]});
			EncounterStartTimes.Remove(StageTime.Key);
			break;
		}
		else
			break;
	}

	// Process active stages
	for (int i = 0; i < ActiveStages.Num(); ++i)
	{
		FActiveEncounterStage& Stage = ActiveStages[i];

		// Update timers
		Stage.TimeLeft -= Delta;
		Stage.ClumpTimer -= Delta;

		// Remove and continue if the stage is over
		if (Stage.TimeLeft <= 0)
		{
			ActiveStages.RemoveAt(i);
			--i;
			continue;
		}

		// Spawning
		if (Stage.ClumpTimer <= 0)
		{
			const double Start = FPlatformTime::Seconds();
			
			// Time to spawn a clump.
			// For each enemy, find a target and a spawn point.
			for (int j = 0; j < Stage.Stage->SpawnsPerClump; j++)
			{
				APiratePlayerCharacter* Target = GetSpawnTarget(); // todo: optimise
				FVector Location;
				UEnemyData* EnemyData = Stage.Stage->Enemies[FMath::RandRange(0, Stage.Stage->Enemies.Num() - 1)];
				if (!GetSpawnPoint(Target, Location, EnemyData->bIsBoss || EnemyData->bIsMiniBoss))
				{
					PIRATE_LOGC_WARN(GetWorld(), "Failed to find a spawn point for enemy!");
					continue;
				}
				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				FTransform TF(Location);
				AEnemy* Enemy = GetWorld()->SpawnActorDeferred<AEnemy>(EnemyData->EnemySubclass, TF, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
				if (!Enemy)
				{
					PIRATE_LOGC_ERROR(GetWorld(), "Failed to spawn enemy!");
					continue;
				}
				FVector Center, Extent;
				Enemy->GetActorBounds(true, Center, Extent);
				Enemy->SetActorLocation(Center + FVector(0, 0, Extent.Z));
				Enemy->FinishSpawning(Enemy->GetTransform());
				Enemy->Multicast_SetData(EnemyData, Target);
				AliveEnemies++;
			}
			Stage.ClumpTimer = Stage.Stage->ClumpRateSeconds;

			const double End = FPlatformTime::Seconds();
			PIRATE_LOGC(GetWorld(), "Time to spawn clump of %d: %f ms", Stage.Stage->SpawnsPerClump, (End - Start) * 1000);
		}
	}
}

APiratePlayerCharacter* UEnemySpawner::GetSpawnTarget() const
{
	double Start = FPlatformTime::Seconds();
	
	int LowestTarget = INT_MAX;
	APiratePlayerCharacter* Target = nullptr;
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APiratePlayerCharacter* Player = Cast<APiratePlayerCharacter>((*It)->GetPawn());
		const APiratePlayerState* PlayerState = Cast<APiratePlayerState>((*It)->PlayerState);
		if (!Player || !PlayerState) 
			continue;

		const int EnemiesTargetingHim = Player->EnemiesTargetingMe - PlayerState->PlayerStats->EnemyAggressionFactor; 
		if (EnemiesTargetingHim < LowestTarget)
		{
			LowestTarget = Player->EnemiesTargetingMe;
			Target = Player;
		}
	}

	double End = FPlatformTime::Seconds();
	PIRATE_LOGC(GetWorld(), "Time to find spawn target: %f ms", (End - Start) * 1000);

	return Target;
}

bool UEnemySpawner::GetSpawnPoint(APiratePlayerCharacter* Target, FVector& OutSpawnPoint, bool bImportant) const
{
	constexpr float MinDist = 2500;
	constexpr float MinDistSquared = FMath::Square(MinDist);
	int Tries = 50;
	
	do
	{
		OutSpawnPoint = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)]->GetActorLocation();
		if (GetLowestSquaredDistanceToPlayer(GetWorld(), OutSpawnPoint) > MinDistSquared)
			return true;
		Tries--;
	} while (Tries > 0);
	
	return false;
	
	// bool bFound = false;
	//
	// constexpr float MinimumDistanceFromAnyPlayer = 2500;
	// constexpr float MinimumDistanceFromAnyPlayerSquared = FMath::Square(MinimumDistanceFromAnyPlayer);
	//
	// int Tries = 10;
	// if (bImportant) // Bodge: ideally, there would just not be a risk of not finding a spawn point
	// 	Tries *= 10;
	// UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	// double Start = FPlatformTime::Seconds();
	// do
	// {
	// 	FVector Dir = FVector(FMath::FRandRange(-1.f, 1.f), FMath::FRandRange(-1.f, 1.f), 0).GetSafeNormal();		
	// 	OutSpawnPoint = Target->GetActorLocation() + (Dir * (MinimumDistanceFromAnyPlayer + 100));
	// 	
	// 	FVector Extent = FVector(200, 200, 200);
	// 	FNavLocation NavLoc;
	// 	bFound = NavSys->ProjectPointToNavigation(OutSpawnPoint, NavLoc, Extent);
	// 	// NavSys->GetRandomPointInNavigableRadius(OutSpawnPoint, 100, NavLoc);
	//
	// 	if (!bFound)
	// 	{
	// 		Tries--;
	// 		continue;
	// 	}
	//
	// 	OutSpawnPoint = NavLoc.Location;
	// 	
	// 	Tries--;
	// } while (Tries > 0 && (!bFound || GetLowestSquaredDistanceToPlayer(GetWorld(), OutSpawnPoint) < MinimumDistanceFromAnyPlayerSquared));
	// double End = FPlatformTime::Seconds();
	// PIRATE_LOGC(GetWorld(), "Time to find spawn point: %f ms", (End - Start) * 1000);
	//
	// return bFound;
}

float UEnemySpawner::GetLowestSquaredDistanceToPlayer(UObject* WorldContextObject, const FVector& Location)
{
	float LowestDistance = FLT_MAX;

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		if (!(*It)->GetPawn())
			continue;
		const float Dist = FVector::DistSquared((*It)->GetPawn()->GetActorLocation(), Location);
		if (Dist < LowestDistance)
			LowestDistance = Dist;
	}

	if (LowestDistance == FLT_MAX)
		return 0;
	return LowestDistance;
}
