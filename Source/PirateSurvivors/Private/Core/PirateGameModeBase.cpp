// copyright lololol

#include <Core/PirateGameModeBase.h>

#include "PirateLog.h"
#include "Core/PirateGameInstance.h"
#include "Core/PirateGameState.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerController.h"
#include "Core/PiratePlayerState.h"
#include "Core/UpgradeList.h"
#include "Weapon/WeaponData.h"
#include "World/XPManager.h"

APirateGameModeBase::APirateGameModeBase()
{
	DefaultPawnClass = APiratePlayerCharacter::StaticClass();
	PlayerControllerClass = APiratePlayerController::StaticClass();
	PlayerStateClass = APiratePlayerState::StaticClass();
	GameStateClass = APirateGameState::StaticClass();
	DefaultPlayerName = FText::FromString("Pirate");
	
	RarityProbabilities.Init(0, static_cast<int>(ERarity::Max));
}

void APirateGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	for (const float RarityProbability : RarityProbabilities)
		RarityProbabilitySum += RarityProbability;
	CreateUpgradeList();
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
		const auto XPItems = APirateGameState::GetPirateGameState(GetWorld())->GetXPManager()->GetCurrentXP();
		if (XPItems.Num() * sizeof(FXPInfo) > 65536)
		{
			PIRATE_LOGC_ERROR(GetWorld(), "XPManager::GetCurrentXP is too large to send to new player over the network!");
			// TODO: Split array and send over multiple RPC calls
		}
		Cast<APiratePlayerController>(NewPlayer)->Client_InitialiseXP(XPItems);
	}
}

void APirateGameModeBase::ListPlayerIndexes() const
{
	auto Players = APirateGameState::GetPirateGameState(GetWorld())->PlayerArray;
	for (int i = 0; i < Players.Num(); i++)
	{
		PIRATE_LOGC_NOLOC(GetWorld(), "Player %d: %s", i, *Players[i]->GetPlayerName());
	}
}

void APirateGameModeBase::ListWeapons() const
{
	const auto Weapons = UPirateGameInstance::GetPirateGameInstance(GetWorld())->GetWeapons();
	for (const auto& Weapon : Weapons)
	{
		PIRATE_LOGC_NOLOC(GetWorld(), "    %s", *Weapon.Key);
	}
}

void APirateGameModeBase::ListEnemies() const
{
	const auto Enemies = UPirateGameInstance::GetPirateGameInstance(GetWorld())->GetEnemies();
	for (const auto& Enemy : Enemies)
	{
		PIRATE_LOGC_NOLOC(GetWorld(), "    %s", *Enemy.Key);
	}
}

void APirateGameModeBase::GivePlayerWeapon(int PlayerIndex, FString WeaponName)
{
	const auto GS = APirateGameState::GetPirateGameState(GetWorld());
	if (PlayerIndex < 0 || PlayerIndex >= GS->PlayerArray.Num())
	{
		PIRATE_LOGC_ERROR_NOLOC(GetWorld(), "Player index %d is out of range", PlayerIndex);
		return;
	}
	const auto Player = GS->PlayerArray[PlayerIndex];
	APiratePlayerState* PlayerState = Cast<APiratePlayerState>(Player.Get());

	if (!PlayerState)
	{
		PIRATE_LOGC_ERROR_NOLOC(GetWorld(), "Couldn't find player with index %d", PlayerIndex);
		return;
	}

	const auto Weapon = UPirateGameInstance::GetPirateGameInstance(GetWorld())->GetWeapon(WeaponName);
	if (!Weapon)
	{
		PIRATE_LOGC_ERROR_NOLOC(GetWorld(), "Couldn't find weapon with name %s", *WeaponName);
		return;
	}
	
	PlayerState->GiveWeaponFromType(Weapon);
}

void APirateGameModeBase::SpawnEnemy(UEnemyData* EnemyType, const FVector& Location, const FRotator& Rotation)
{
	// TODO this should be in the game state
}

void APirateGameModeBase::SpawnEnemyNearby(const FString& EnemyType)
{
	const auto Enemy = UPirateGameInstance::GetPirateGameInstance(GetWorld())->GetEnemy(EnemyType);
	if (!Enemy)
	{
		PIRATE_LOGC_ERROR_NOLOC(GetWorld(), "Couldn't find enemy with name %s", *EnemyType);
		return;
	}

	const auto LocalPawn = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->GetPawn();
	if (!LocalPawn)
	{
		PIRATE_LOGC_ERROR_NOLOC(GetWorld(), "Couldn't find local pawn");
		return;
	}

	// TODO: This should find a valid nearby location on the nav
	SpawnEnemy(Enemy, LocalPawn->GetActorLocation() + FVector(300, 0, 0), FRotator::ZeroRotator);
}

ERarity APirateGameModeBase::GetRarityForPlayer(APiratePlayerState* Player) const
{
	float Roll = FMath::FRandRange(0, RarityProbabilitySum);
	Roll += FMath::FRandRange(0, Player->PlayerStats->Luck * 0.05f * RarityProbabilitySum);
	float Accum = 0;
	for (int i = 0; i < RarityProbabilities.Num(); i++)
	{
		if (Roll < Accum + RarityProbabilities[i])
			return static_cast<ERarity>(static_cast<int>(ERarity::Max) - i);
		Accum += RarityProbabilities[i];
	}
	return ERarity::Common; // Should never happen
}

void APirateGameModeBase::CreateUpgradeList_Implementation()
{
	UpgradeList = NewObject<UUpgradeList>(this);
}
