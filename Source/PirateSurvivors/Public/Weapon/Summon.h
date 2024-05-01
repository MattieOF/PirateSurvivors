// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Summon.generated.h"

class UWeaponStats;
class APirateSurvivorsCharacter;

UCLASS()
class PIRATESURVIVORS_API ASummon : public AActor
{
	GENERATED_BODY()

public:
	ASummon();

	UPROPERTY(BlueprintReadWrite, Category = "Summon")
	APirateSurvivorsCharacter* OwningCharacter;

	UPROPERTY(BlueprintReadWrite, Category = "Summon")
	UWeaponStats* Stats;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnSpawn();

	UFUNCTION(BlueprintNativeEvent)
	void OnDie();
};
