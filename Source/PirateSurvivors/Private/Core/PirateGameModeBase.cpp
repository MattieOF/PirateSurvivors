// copyright lololol

#include <Core/PirateGameModeBase.h>

#include "PirateLog.h"
#include "Core/PirateGameInstance.h"
#include "Core/PirateGameState.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerController.h"
#include "Core/PiratePlayerState.h"
#include "Engine/Console.h"
#include "Weapon/WeaponData.h"
#include "World/XPManager.h"

APirateGameModeBase::APirateGameModeBase()
{
	DefaultPawnClass = APiratePlayerCharacter::StaticClass();
	PlayerControllerClass = APiratePlayerController::StaticClass();
	PlayerStateClass = APiratePlayerState::StaticClass();
	GameStateClass = APirateGameState::StaticClass();
	DefaultPlayerName = FText::FromString("Pirate");
}

void APirateGameModeBase::ListPlayerIndexes() const
{
	auto Players = APirateGameState::GetPirateGameState(GetWorld())->PlayerArray;
	for (int i = 0; i < Players.Num(); i++)
	{
		FString Output = FString::Printf(TEXT("Player %d: %s"), i, *Players[i]->GetPlayerName());
		PIRATE_LOG_S(Output);
		GetWorld()->GetGameViewport()->ViewportConsole->OutputText(Output);
	}
}

void APirateGameModeBase::ListWeapons() const
{
	const auto Weapons = UPirateGameInstance::GetPirateGameInstance(GetWorld())->GetWeapons();
	for (const auto& Weapon : Weapons)
	{
		FString Output = FString::Printf(TEXT("    %s"), *Weapon.Key);
		PIRATE_LOG_S(Output);
		GetWorld()->GetGameViewport()->ViewportConsole->OutputText(Output);
	}
}

void APirateGameModeBase::ListEnemies() const
{
	const auto Enemies = UPirateGameInstance::GetPirateGameInstance(GetWorld())->GetEnemies();
	for (const auto& Enemy : Enemies)
	{
		FString Output = FString::Printf(TEXT("    %s"), *Enemy.Key);
		PIRATE_LOG_S(Output);
		GetWorld()->GetGameViewport()->ViewportConsole->OutputText(Output);
	}
}

void APirateGameModeBase::GivePlayerWeapon(int PlayerIndex, FString WeaponName)
{
	const auto GS = APirateGameState::GetPirateGameState(GetWorld());
	if (PlayerIndex < 0 || PlayerIndex >= GS->PlayerArray.Num())
	{
		const FString Error = FString::Printf(TEXT("Player index %d is out of range"), PlayerIndex);
		PIRATE_LOG_ERROR_S(Error);
		GetWorld()->GetGameViewport()->ViewportConsole->OutputText(Error);
		return;
	}
	const auto Player = GS->PlayerArray[PlayerIndex];
	APiratePlayerState* PlayerState = Cast<APiratePlayerState>(Player.Get());

	if (!PlayerState)
	{
		const FString Error = FString::Printf(TEXT("Couldn't find player with index %d"), PlayerIndex);
		PIRATE_LOG_ERROR_S(Error);
		GetWorld()->GetGameViewport()->ViewportConsole->OutputText(Error);
		return;
	}

	const auto Weapon = UPirateGameInstance::GetPirateGameInstance(GetWorld())->GetWeapon(WeaponName);
	if (!Weapon)
	{
		const FString Error = FString::Printf(TEXT("Couldn't find weapon with name %s"), *WeaponName);
		PIRATE_LOG_ERROR_S(Error);
		GetWorld()->GetGameViewport()->ViewportConsole->OutputText(Error);
		return;
	}
	
	PlayerState->GiveWeaponFromType(Weapon);
}

void APirateGameModeBase::SpawnEnemy(UEnemyData* EnemyType, const FVector& Location, const FRotator& Rotation)
{
	// TODO this should be in the game state
}

void APirateGameModeBase::SpawnEnemyNearby(FString EnemyType)
{
	const auto Enemy = UPirateGameInstance::GetPirateGameInstance(GetWorld())->GetEnemy(EnemyType);
	if (!Enemy)
	{
		const FString Error = FString::Printf(TEXT("Couldn't find enemy with name %s"), *EnemyType);
		PIRATE_LOG_ERROR_S(Error);
		GetWorld()->GetGameViewport()->ViewportConsole->OutputText(Error);
		return;
	}

	const auto LocalPawn = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->GetPawn();
	if (!LocalPawn)
	{
		const FString Error = FString::Printf(TEXT("Couldn't find local pawn"));
		PIRATE_LOG_ERROR_S(Error);
		GetWorld()->GetGameViewport()->ViewportConsole->OutputText(Error);
		return;
	}

	// TODO: This should find a valid nearby location on the nav
	SpawnEnemy(Enemy, LocalPawn->GetActorLocation() + FVector(300, 0, 0), FRotator::ZeroRotator);
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
