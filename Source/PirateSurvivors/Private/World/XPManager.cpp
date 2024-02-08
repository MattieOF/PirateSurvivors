// copyright lololol

#include "World/XPManager.h"

#include "PirateLog.h"
#include "Core/PirateGameState.h"
#include "Core/PiratePlayerCharacter.h"
#include "World/XP.h"

AXPManager::AXPManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	bAlwaysRelevant = true;
}

int AXPManager::GetFreeID()
{
	int ID;
	do {
		ID = FMath::RandHelper(INT_MAX - 1);
	} while (ID != 0 && CurrentXPObjects.Contains(ID));
	return ID;
}

TArray<FXPInfo> AXPManager::GetCurrentXP()
{
	double Start = FPlatformTime::Seconds();
	
	TArray<FXPInfo> XPItems;
	for (const auto XP : CurrentXPObjects)
	{
		FXPInfo Info;
		Info.Location = XP.Value->GetActorLocation();
		Info.Value = XP.Value->Value;
		Info.ID = XP.Key;
		XPItems.Add(Info);
	}

	double End = FPlatformTime::Seconds();
	PIRATE_LOG(FString::Printf(TEXT("XPManager::GetCurrentXP took %f ms"), (End - Start) * 1000));
	
	return XPItems;
}

void AXPManager::Initialise(TArray<FXPInfo> XPItems, bool bClearFirst)
{
	if (bClearFirst)
	{
		for (const auto& XP : CurrentXPObjects)
			if (XP.Value->IsValidLowLevel()) XP.Value->Destroy();
		CurrentXPObjects.Empty();
	}
	
	for (auto [Location, Value, ID] : XPItems)
	{
		if (CurrentXPObjects.Contains(ID))
		{
			PIRATE_LOG_ERROR("XPManager::Initialise: XP with ID %d already exists! Skipping...");
			continue;
		}
		
		AXP* XP = GetWorld()->SpawnActorDeferred<AXP>(APirateGameState::GetPirateGameState(GetWorld())->XPClass, FTransform(Location));
		XP->Value = Value;
		XP->ID = ID;
		XP->FinishSpawning(XP->GetTransform());
		CurrentXPObjects.Add(ID, XP);
	}
}

void AXPManager::Multicast_PickupXP_Implementation(APiratePlayerCharacter* Character, int XPID)
{
	if (!CurrentXPObjects.Contains(XPID))
	{
		PIRATE_LOG_ERROR(FString::Printf(TEXT("Tried to pick up XP with ID %d, but it doesn't exist!"), XPID));
		return;
	}

	AXP* XP = CurrentXPObjects[XPID];
	XP->bPickedUp = true;
	XP->SetActorEnableCollision(false);
	Character->XPBeingPickedUp.Add(XP);
}

void AXPManager::Multicast_DestroyXP_Implementation(int ID)
{
	if (!CurrentXPObjects.Contains(ID))
	{
		// PIRATE_LOG_ERROR(FString::Printf(TEXT("Tried to destroy XP with ID %d, but it doesn't exist!"), ID));
		return;
	}
	
	AXP* XP = CurrentXPObjects[ID];
	XP->Destroy();
	CurrentXPObjects.Remove(ID);
}

void AXPManager::Multicast_SpawnXP_Implementation(FVector Location, float Value, int ID)
{
	AXP* XP = GetWorld()->SpawnActorDeferred<AXP>(APirateGameState::GetPirateGameState(GetWorld())->XPClass, FTransform(Location));
	XP->Value = Value;
	XP->ID = ID;
	XP->FinishSpawning(XP->GetTransform());

	if (CurrentXPObjects.Contains(ID))
	{
		PIRATE_LOG_ERROR(FString::Printf(TEXT("Tried to spawn XP with ID %d, but it already exists!"), ID));
	}
	CurrentXPObjects.Add(ID, XP);
}
