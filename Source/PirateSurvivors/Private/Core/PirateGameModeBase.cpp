// copyright lololol

#include <Core/PirateGameModeBase.h>

#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerController.h"

APirateGameModeBase::APirateGameModeBase()
{
	DefaultPawnClass = APiratePlayerCharacter::StaticClass();
	PlayerControllerClass = APiratePlayerController::StaticClass();
	DefaultPlayerName = FText::FromString("Pirate");
}
