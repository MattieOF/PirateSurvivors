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
	FVector Location = FVector(0, 0, 0);
	
	UPROPERTY(BlueprintReadWrite)
	float Value = 0;
	
	UPROPERTY(BlueprintReadWrite)
	int ID = 0;
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

	UFUNCTION(BlueprintCallable)
	void DestroyXP(int ID);

	UFUNCTION(BlueprintCallable)
	void SpawnXP(FVector Location, float Value, int ID = 0);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_SpawnXP(FVector Location, float Value, int ID = 0);
	
	UFUNCTION(BlueprintCallable)
	void SpawnXPWithPhysics(FVector Location, float Value, int ID = 0, FVector Velocity = FVector::ZeroVector);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_SpawnXPWithPhysics(FVector Location, float Value, int ID = 0, FVector Velocity = FVector::ZeroVector);
	
	UFUNCTION(BlueprintCallable)
	void SpawnSpecialXPWithPhysics(TSubclassOf<AXP> SpecialClass, FVector Location, float Value, int ID = 0, FVector Velocity = FVector::ZeroVector);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_SpawnSpecialXPWithPhysics(TSubclassOf<AXP> SpecialClass, FVector Location, float Value, int ID = 0, FVector Velocity = FVector::ZeroVector);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_PickupXP(APiratePlayerCharacter* Character, int XPID, int Tries = 3);
	
	TMap<int, AXP*> CurrentXPObjects;

protected:
	virtual void BeginPlay() override;

	bool bHasInitialised = false;
};
