// copyright lololol

#include "Core/PirateGameState.h"

#include "Net/UnrealNetwork.h"
#include "World/DamageNumberManager.h"
#include "World/XPManager.h"

void APirateGameState::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
		XPManager = GetWorld()->SpawnActor<AXPManager>();
	DamageNumberManager = GetWorld()->SpawnActor<ADamageNumberManager>(); // Every client spawns their own DamageNumberManager
}

void APirateGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APirateGameState, XPManager);
}
