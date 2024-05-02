// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EncounterData.generated.h"

class UEnemyData;

USTRUCT(BlueprintType)
struct FEncounterStage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Stage")
	float StartTimeSeconds = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Stage")
	float LengthSeconds = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Stage")
	int SpawnsPerClump = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Stage")
	float ClumpRateSeconds = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Stage")
	TArray<UEnemyData*> Enemies;
};

UCLASS()
class PIRATESURVIVORS_API UEncounterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Data")
	FText Name = FText::FromString("New Encounter");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter Data")
	TArray<FEncounterStage> Stages;
};
