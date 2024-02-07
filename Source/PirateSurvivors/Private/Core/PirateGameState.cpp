// copyright lololol

#include "Core/PirateGameState.h"

#include "World/XPManager.h"

void APirateGameState::BeginPlay()
{
	Super::BeginPlay();
	XPManager = GetWorld()->SpawnActor<AXPManager>();
}
