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
	int ID = 0;
	do {
		ID = FMath::RandHelper(INT_MAX - 1);
	} while (ID != 0 && CurrentXPObjects.Contains(ID));
	return ID;
}

void AXPManager::PickupXP_Implementation(APiratePlayerCharacter* Character, int XPID)
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

void AXPManager::DestroyXP_Implementation(int ID)
{
	if (!CurrentXPObjects.Contains(ID))
	{
		PIRATE_LOG_ERROR(FString::Printf(TEXT("Tried to destroy XP with ID %d, but it doesn't exist!"), ID));
		return;
	}
	
	AXP* XP = CurrentXPObjects[ID];
	XP->Destroy();
	CurrentXPObjects.Remove(ID);
}

void AXPManager::SpawnXP_Implementation(FVector Location, float Value, int ID)
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
