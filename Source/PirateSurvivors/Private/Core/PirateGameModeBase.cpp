// copyright lololol

#include <Core/PirateGameModeBase.h>

#include "PirateLog.h"
#include "Core/PirateGameState.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerController.h"
#include "Core/PiratePlayerState.h"
#include "World/XPManager.h"

APirateGameModeBase::APirateGameModeBase()
{
	DefaultPawnClass = APiratePlayerCharacter::StaticClass();
	PlayerControllerClass = APiratePlayerController::StaticClass();
	PlayerStateClass = APiratePlayerState::StaticClass();
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
	
	if (!NewPlayer->PlayerState)
	{
		PIRATE_LOG_ERROR("Player state is null!");
	}
	
	Cast<APiratePlayerController>(NewPlayer)->Client_CallCreateUI();

	// Send current XP items to new player
	if (!NewPlayer->IsLocalController())
	{
		auto XPItems = APirateGameState::GetPirateGameState(GetWorld())->GetXPManager()->GetCurrentXP();
		if (XPItems.Num() * sizeof(FXPInfo) > 65536)
		{
			PIRATE_LOG_ERROR("XPManager::GetCurrentXP is too large to send to new player over the network!");
			// TODO: Split array and send over multiple RPC calls
		}
		Cast<APiratePlayerController>(NewPlayer)->Client_InitialiseXP(XPItems);
	}
}
