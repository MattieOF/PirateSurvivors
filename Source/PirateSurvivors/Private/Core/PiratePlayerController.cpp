// copyright lololol

#include "Core/PiratePlayerController.h"

#include "PirateLog.h"
#include "Core/PirateGameInstance.h"
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

void APiratePlayerController::EnsureXPReplicated()
{
	// If XPManager is initialised, then replicate the XP items
	if (const auto GS = APirateGameState::GetPirateGameState(GetWorld()))
	{
		if (const auto XPManager = GS->GetXPManager())
		{
			const auto GI = UPirateGameInstance::GetPirateGameInstance(GetWorld());
			XPManager->Initialise(GI->XPToBeReplicated, false);
			GI->XPToBeReplicated.Empty();
			return;
		}
	}
	
	// Couldn't replicate yet, so set timer for next tick
	GetWorldTimerManager().SetTimerForNextTick(this, &APiratePlayerController::EnsureXPReplicated);
}

void APiratePlayerController::Client_InitialiseXP_Implementation(const TArray<FXPInfo>& XPItems)
{
	// If XPManger is already initialised, then initialise the XP items
	if (auto GS = APirateGameState::GetPirateGameState(GetWorld()))
	{
		if (GS->GetXPManager())
		{
			GS->GetXPManager()->Initialise(XPItems);
			return;	
		}
	}

	// Otherwise, store the XP items in the game instance to be replicated once the XPManager is initialised
	// TODO: This can probably be removed
	UPirateGameInstance::GetPirateGameInstance(GetWorld())->XPToBeReplicated.Append(XPItems);
	EnsureXPReplicated();
}

void APiratePlayerController::Client_CallCreateUI_Implementation()
{
	CreateUI();
}
