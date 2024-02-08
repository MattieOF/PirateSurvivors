// copyright lololol

#include "Core/PiratePlayerController.h"

#include "PirateLog.h"
#include "Core/PirateGameState.h"
#include "Core/PiratePlayerCharacter.h"
#include "World/XPManager.h"

void APiratePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Pirate = Cast<APiratePlayerCharacter>(InPawn);
	if (!Pirate)
	{
		PIRATE_LOG_ERROR("Pirate player controller has possessed non Pirate character!");
	}
}

void APiratePlayerController::Client_InitialiseXP_Implementation(const TArray<FXPInfo>& XPItems)
{
	APirateGameState::GetPirateGameState(GetWorld())->GetXPManager()->Initialise(XPItems);
}

void APiratePlayerController::Client_CallCreateUI_Implementation()
{
	CreateUI();
}
