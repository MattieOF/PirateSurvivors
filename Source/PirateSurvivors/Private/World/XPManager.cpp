// copyright lololol

#include "World/XPManager.h"

#include "PirateLog.h"
#include "Core/PirateGameInstance.h"
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
	PIRATE_LOG_NOLOC("XPManager::GetCurrentXP took %f ms", (End - Start) * 1000);
	
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
			PIRATE_LOG_WARN_NOLOC("XPManager::Initialise: XP with ID %d already exists! Skipping...");
			continue;
		}
		
		AXP* XP = GetWorld()->SpawnActorDeferred<AXP>(APirateGameState::GetPirateGameState(GetWorld())->XPClass, FTransform(Location));
		XP->Value = Value;
		XP->ID = ID;
		XP->FinishSpawning(XP->GetTransform());
		CurrentXPObjects.Add(ID, XP);
	}

	bHasInitialised = true;
}

void AXPManager::DestroyXP(int ID)
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

void AXPManager::SpawnXP(FVector Location, float Value, int ID)
{
	if (!HasAuthority()) return;
	if (ID == 0)
		ID = GetFreeID();
	Multicast_SpawnXP(Location, Value, ID);
}

void AXPManager::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() != NM_Client)
		bHasInitialised = true; // Server doesn't need to wait for anything

	UPirateGameInstance* const GameInstance = UPirateGameInstance::GetPirateGameInstance(GetWorld());
	if (GameInstance->XPToBeReplicated.Num() > 0)
	{
		Initialise(GameInstance->XPToBeReplicated, false);
		GameInstance->XPToBeReplicated.Empty();
	}
}

void AXPManager::Multicast_PickupXP_Implementation(APiratePlayerCharacter* Character, int XPID, int Tries)
{
	if (!CurrentXPObjects.Contains(XPID))
	{
		if (GetNetMode() != NM_Client || Tries <= 0)
		{
			PIRATE_LOG_ERROR_NOLOC("Tried to pick up XP with ID %d, but it doesn't exist!", XPID);
			return;
		}
		
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, [this, Character, XPID, Tries] { Multicast_PickupXP_Implementation(Character, XPID, Tries - 1); }, 1.f, false, -1 );
		return;
	}

	// This can happen if XP is picked up as the client is joining the game
	if (!Character)
	{
		// Character is null, so we've received the RPC before the client has fully joined the game
		// In this case, the best thing we can do is just destroy the XP object, since there's no way for us to
		// figure out which player should pick it up.
		Multicast_DestroyXP(XPID);
		return;
	}

	AXP* XP = CurrentXPObjects[XPID];
	if (XP->bPickedUp)
		return;
	XP->bPickedUp = true;
	XP->SetActorEnableCollision(false);
	Character->XPBeingPickedUp.Add(XP);
}

void AXPManager::Multicast_DestroyXP_Implementation(int ID)
{
	DestroyXP(ID);
}

void AXPManager::Multicast_SpawnXP_Implementation(FVector Location, float Value, int ID)
{
	if (CurrentXPObjects.Contains(ID))
	{
		PIRATE_LOG_ERROR_NOLOC("Tried to spawn XP with ID %d, but it already exists!", ID);
		return;
	}

	const auto GS = APirateGameState::GetPirateGameState(GetWorld());
	if (!GS || !GS->XPClass) return;
	AXP* XP = GetWorld()->SpawnActorDeferred<AXP>(GS->XPClass, FTransform(Location));
	XP->Value = Value;
	XP->ID = ID;
	// We add it before FinishSpawning as FinishSpawning will cause collisions to be checked, so if a player
	// is in range of the XP when it spawns, they'll pick it up immediately. This will call Multicast_PickupXP before
	// the ID is added to the map, which will cause an error.
	CurrentXPObjects.Add(ID, XP);
	XP->FinishSpawning(XP->GetTransform());
}
