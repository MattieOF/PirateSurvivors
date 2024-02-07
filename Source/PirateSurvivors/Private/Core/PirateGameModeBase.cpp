// copyright lololol

#include <Core/PirateGameModeBase.h>

#include "Core/PirateGameState.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerController.h"
#include "World/XPManager.h"

APirateGameModeBase::APirateGameModeBase()
{
	DefaultPawnClass = APiratePlayerCharacter::StaticClass();
	PlayerControllerClass = APiratePlayerController::StaticClass();
	GameStateClass = APirateGameState::StaticClass();
	DefaultPlayerName = FText::FromString("Pirate");
}

void APirateGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void APirateGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	Cast<APiratePlayerController>(NewPlayer)->CallCreateUI();
	// TODO: Replicate existing XP to new player
	// Send array of structs with XP Pos, ID, and value?
	// RPC call sends one UDP packet - so a max of 64(ish) KB.
	// 3 floats, 2 ints per XP object = 20 bytes per XP object
	// 64 KB = 65536 bytes
	// 65536 / 20 = 3276 XP objects
	// Should be plenty for now
	// But if it wasn't, we could split the array into chunks and send them one at a time
}
