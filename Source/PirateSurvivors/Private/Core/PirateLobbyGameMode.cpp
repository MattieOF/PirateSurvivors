// copyright lololol

#include "Core/PirateLobbyGameMode.h"

#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerController.h"
#include "Core/PiratePlayerState.h"

APirateLobbyGameMode::APirateLobbyGameMode()
{
	DefaultPawnClass = APiratePlayerCharacter::StaticClass();
	PlayerControllerClass = APiratePlayerController::StaticClass();
	PlayerStateClass = APiratePlayerState::StaticClass();
	// GameStateClass = APirateGameState::StaticClass();
	DefaultPlayerName = FText::FromString("Pirate");
}

void APirateLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	Cast<APiratePlayerController>(NewPlayer)->Client_CallCreateUI();
}

void APirateLobbyGameMode::AddReadiedPlayer(APiratePlayerState* PlayerState)
{
	if (ReadiedPlayers.Contains(PlayerState))
		return;
	ReadiedPlayers.Add(PlayerState);
	
	ReadyPlayers++;

	if (ReadyPlayers >= 2)
	{
		GetWorld()->ServerTravel(MapToLoad);
	}
}
