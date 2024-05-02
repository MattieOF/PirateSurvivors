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

	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, Category = "Summon")
	APirateSurvivorsCharacter* OwningCharacter;

	UPROPERTY(BlueprintReadWrite, Category = "Summon")
	UWeaponStats* Stats;

	UFUNCTION(BlueprintCallable, Category = "Summon", NetMulticast, Reliable)
	void Multicast_Die();
	
	UFUNCTION(BlueprintCallable, Category = "Summon")
	void Die();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnSpawn();

	UFUNCTION(BlueprintNativeEvent)
	void OnDie();
};
