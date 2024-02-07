// copyright lololol

#include "Core/PiratePlayerController.h"

#include "PirateLog.h"
#include "Core/PiratePlayerCharacter.h"

void APiratePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Pirate = Cast<APiratePlayerCharacter>(InPawn);
	if (!Pirate)
	{
		PIRATE_LOG_ERROR("Pirate player controller has possessed non Pirate character!");
	}
}

void APiratePlayerController::CallCreateUI_Implementation()
{
	CreateUI();
}
