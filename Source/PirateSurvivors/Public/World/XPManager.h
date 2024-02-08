// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XPManager.generated.h"

class APiratePlayerCharacter;
class AXP;

USTRUCT(BlueprintType)
struct FXPInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector Location;
	
	UPROPERTY(BlueprintReadWrite)
	float Value;
	
	UPROPERTY(BlueprintReadWrite)
	int ID;
};

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FXPInfo> GetCurrentXP();
	
	UFUNCTION(BlueprintCallable)
	void Initialise(TArray<FXPInfo> XPItems, bool bClearFirst = false);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_DestroyXP(int ID);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_SpawnXP(FVector Location, float Value, int ID);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_PickupXP(APiratePlayerCharacter* Character, int XPID);
	
	TMap<int, AXP*> CurrentXPObjects;

protected:
	virtual void BeginPlay() override;
};
