// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XPManager.generated.h"

class APiratePlayerCharacter;
class AXP;

// Manages all XP objects in the world
// It is responsible for spawning and destroying XP, while avoiding replication for each XP object
// (there are only 2048 network channels available, so we can't replicate every single XP object. We need to manage them manually)
// This actor is replicated, and has RPCs to spawn and destroy XP objects
UCLASS()
class PIRATESURVIVORS_API AXPManager : public AActor
{
	GENERATED_BODY()

public:
	AXPManager();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetFreeID();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void DestroyXP(int ID);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void SpawnXP(FVector Location, float Value, int ID);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void PickupXP(APiratePlayerCharacter* Character, int XPID);
	
	TMap<int, AXP*> CurrentXPObjects;
};
