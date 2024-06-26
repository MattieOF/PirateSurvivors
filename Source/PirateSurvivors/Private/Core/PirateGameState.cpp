﻿// copyright lololol

#include "Core/PirateGameState.h"

#include "PirateLog.h"
#include "Blueprint/UserWidget.h"
#include "Core/PirateGameInstance.h"
#include "Core/PiratePlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "UI/DamageNumbers.h"
#include "UI/HealthBars.h"
#include "World/XPManager.h"

void APirateGameState::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
		XPManager = GetWorld()->SpawnActor<AXPManager>();

	const auto GI = UPirateGameInstance::GetPirateGameInstance(GetWorld());

	// Create the damage numbers widget
	DamageNumbers = Cast<UDamageNumbers>(CreateWidget(GI, GI->DamageNumbersClass));
	if (!DamageNumbers)
		PIRATE_LOGC_ERROR(GetWorld(), "GI DamageNumbersClass is either unset or not a subclass of UDamageNumbers");
	else
		DamageNumbers->AddToViewport(-100);

	// Create the health bars widget
	HealthBars = Cast<UHealthBars>(CreateWidget(GI, GI->HealthBarsClass));
	if (!HealthBars)
		PIRATE_LOGC_ERROR(GetWorld(), "GI HealthBarsClass is either unset or not a subclass of UHealthBars");
	else
		HealthBars->AddToViewport(-101);
}

void APirateGameState::OnPlayerDied()
{
	if (!HasAuthority())
		return;
	
	// iterate through all players, if all are dead, game over
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto Player = Cast<APiratePlayerCharacter>(It->Get()->GetPawn());
		if (Player && !Player->GetHealthComponent()->IsDead())
			return;
	}

	Multicast_GameOver();
}

void APirateGameState::Multicast_GameOver_Implementation()
{
	OnGameOver.Broadcast();
}

void APirateGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APirateGameState, XPManager);
}
