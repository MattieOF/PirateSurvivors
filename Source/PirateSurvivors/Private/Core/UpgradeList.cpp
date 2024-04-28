// copyright lololol

#include "Core/UpgradeList.h"

#include "PirateLog.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/PirateGameModeBase.h"
#include "Core/PiratePlayerState.h"
#include "Player/Upgrade.h"

UUpgradeList::UUpgradeList()
{
	// Get the asset registry module
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
		FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Since asset registry scanning is asyncronous, we initiate a synchronous scan 
	// of the data directories to ensure they've all been discovered.
	TArray<FString> ContentPaths;
	ContentPaths.Add(TEXT("/Game/Upgrades"));
	AssetRegistry.ScanPathsSynchronous(ContentPaths);

	// Find all the weapon upgrade assets and add them to the weapon upgrades map
	TArray<FAssetData> AssetData;
	{ // Scope to avoid copypasta errors
		const UClass* WeaponUpgradeClass = UWeaponUpgrade::StaticClass();
		AssetRegistry.GetAssetsByClass(FTopLevelAssetPath(WeaponUpgradeClass->GetPackage()->GetFName(), WeaponUpgradeClass->GetFName()), AssetData);
		for (auto& Asset : AssetData)
		{
			UWeaponUpgrade* Upgrade = Cast<UWeaponUpgrade>(Asset.GetAsset());
			if (!WeaponUpgrades.Contains(Upgrade->Rarity))
				WeaponUpgrades.Add(Upgrade->Rarity, TArray<UWeaponUpgrade*>());
			WeaponUpgrades[Upgrade->Rarity].Add(Upgrade);
			TotalWeaponUpgrades++;
		}
	}

	// And then the same for player upgrades
	{
		AssetData.Empty();
		const UClass* PlayerUpgradeClass = UPlayerUpgrade::StaticClass();
		AssetRegistry.GetAssetsByClass(FTopLevelAssetPath(PlayerUpgradeClass->GetPackage()->GetFName(), PlayerUpgradeClass->GetFName()), AssetData);
		for (auto& Asset : AssetData)
		{
			UPlayerUpgrade* Upgrade = Cast<UPlayerUpgrade>(Asset.GetAsset());
			if (!PlayerUpgrades.Contains(Upgrade->Rarity))
				PlayerUpgrades.Add(Upgrade->Rarity, TArray<UPlayerUpgrade*>());
			PlayerUpgrades[Upgrade->Rarity].Add(Upgrade);
			TotalPlayerUpgrades++;
		}
	}
}

TArray<UWeaponUpgrade*> UUpgradeList::GetAllWeaponUpgrades()
{
	TArray<UWeaponUpgrade*> AllUpgrades;
	AllUpgrades.Reserve(TotalWeaponUpgrades);
	for (auto& [Rarity, Upgrades] : WeaponUpgrades)
		AllUpgrades.Append(Upgrades);
	return AllUpgrades;
}

TArray<UWeaponUpgrade*> UUpgradeList::GetAllWeaponUpgradesOfRarity(const ERarity Rarity) const
{
	return WeaponUpgrades.FindRef(Rarity);
}

TArray<UPlayerUpgrade*> UUpgradeList::GetAllPlayerUpgrades()
{
	TArray<UPlayerUpgrade*> AllUpgrades;
	AllUpgrades.Reserve(TotalPlayerUpgrades);
	for (auto& [Rarity, Upgrades] : PlayerUpgrades)
		AllUpgrades.Append(Upgrades);
	return AllUpgrades;
}

TArray<UPlayerUpgrade*> UUpgradeList::GetAllPlayerUpgradesOfRarity(const ERarity Rarity) const
{
	return PlayerUpgrades.FindRef(Rarity);
}

TArray<FQueuedUpgradeChoice> UUpgradeList::GetPlayerUpgradeChoices(APiratePlayerState* Player) const
{
	const double Start = FPlatformTime::Seconds();
	
	TArray<FQueuedUpgradeChoice> Choices;
	TArray<UUpgrade*> ChosenSoFar;
	const int ChoiceCount = Player->PlayerStats->UpgradeChoices;
	Choices.Reserve(ChoiceCount);
	
	for (int i = 0; i < ChoiceCount; i++)
	{
		const bool WeaponUpgrade = FMath::RandBool();
		UUpgrade* Choice = nullptr;

		// First, try and get a type of the type we rolled.
		if (!WeaponUpgrade)
			Choice = GetPlayerPlayerUpgradeChoice(Player, ChosenSoFar);
		else
			Choice = GetPlayerWeaponUpgradeChoice(Player, ChosenSoFar);

		// If for some reason we couldn't get one of those, try get the other type
		if (Choice == nullptr)
		{
			if (!WeaponUpgrade)
				Choice = GetPlayerWeaponUpgradeChoice(Player, ChosenSoFar);
			else
				Choice = GetPlayerPlayerUpgradeChoice(Player, ChosenSoFar);
		}

		// If we still couldn't get one, just return what we have
		if (Choice == nullptr)
		{
			return Choices;
		}

		// We did get one, so add it to the list of choices
		ChosenSoFar.Add(Choice);
		int TargetIndex = -1;
		if (UWeaponUpgrade* Upgrade = Cast<UWeaponUpgrade>(Choice))
		{
			if (Upgrade->bForOneWeapon)
			{
				// TODO: We've already calculated this before during the initial upgrade selection, so it's wasteful to do it again.
				TArray<AWeaponFunctionality*> Weapons = *Player->GetWeapons();
				Weapons.FilterByPredicate([&Upgrade](AWeaponFunctionality* Weapon) { return Upgrade->IsValidForWeapon(Weapon); });
				if (Weapons.Num() > 0)
					TargetIndex = FMath::RandRange(0, Weapons.Num() - 1);
				else
					PIRATE_LOGC_ERROR(GetWorld(), "Somehow no valid weapon?");
			}
		}
		Choices.Add({ Choice, TargetIndex });
	}

	const double End = FPlatformTime::Seconds();
	PIRATE_LOGC(GetWorld(), "GetPlayerUpgradeChoices took %fms", (End - Start) * 1000);
	
	return Choices;
}

UPlayerUpgrade* UUpgradeList::GetPlayerPlayerUpgradeChoice(APiratePlayerState* Player, const TArray<UUpgrade*>& Blacklist) const
{
	const APirateGameModeBase* GameMode = APirateGameModeBase::GetPirateGameMode(Player->GetWorld());

	UPlayerUpgrade* Choice = nullptr;
	bool HasTriedAboveRarities = false;
	ERarity Rarity = GameMode->GetRarityForPlayer(Player);
	do 
	{
		TArray<UPlayerUpgrade*> Upgrades = GetAllPlayerUpgradesOfRarity(Rarity);
		// TODO: @optimization: we could cache the filtered upgrades per rarity
		Upgrades = Upgrades.FilterByPredicate([&Player, &Blacklist](const UPlayerUpgrade* Upgrade) { return Upgrade->IsValidForPlayer(Player) && !Blacklist.Contains(Cast<UUpgrade>(Upgrade)); });
		if (Upgrades.Num() > 0)
		{
			Choice = Upgrades[FMath::RandRange(0, Upgrades.Num() - 1)];
			break;
		}
			
		// Go up a rarity if !HasTriedAboveRarities, otherwise go down
		Rarity = static_cast<ERarity>(static_cast<int>(Rarity) - (HasTriedAboveRarities ? 1 : -1));
		if (static_cast<int>(Rarity) >= static_cast<int>(ERarity::Max) - 1)
		{
			HasTriedAboveRarities = true;
			Rarity = static_cast<ERarity>(static_cast<int>(ERarity::Max) - 1);
		}
	} while ((Choice == nullptr || !PlayerUpgrades.Contains(Rarity)) && !(static_cast<int>(Rarity) == 0 && HasTriedAboveRarities));

	return Choice;
}

UWeaponUpgrade* UUpgradeList::GetPlayerWeaponUpgradeChoice(APiratePlayerState* Player, const TArray<UUpgrade*>& Blacklist) const
{
	const APirateGameModeBase* GameMode = APirateGameModeBase::GetPirateGameMode(Player->GetWorld());

	UWeaponUpgrade* Choice = nullptr;
	bool HasTriedAboveRarities = false;
	ERarity Rarity = GameMode->GetRarityForPlayer(Player);
	do 
	{
		TArray<UWeaponUpgrade*> Upgrades = GetAllWeaponUpgradesOfRarity(Rarity);
		// TODO: @optimization: we could cache the filtered upgrades per rarity
		// This is even more important for weapon upgrades, since their IsValidForPlayer is more expensive
		// @copy-paste: this has a lot of duplicated code with GetPlayerPlayerUpgradeChoice
		Upgrades = Upgrades.FilterByPredicate([&Player, &Blacklist](const UWeaponUpgrade* Upgrade) { return Upgrade->IsValidForPlayer(Player) && !Blacklist.Contains(Cast<UUpgrade>(Upgrade)); });
		if (Upgrades.Num() > 0)
		{
			Choice = Upgrades[FMath::RandRange(0, Upgrades.Num() - 1)];
			break;
		}
		
		// Go up a rarity if !HasTriedAboveRarities, otherwise go down
		Rarity = static_cast<ERarity>(static_cast<int>(Rarity) - (HasTriedAboveRarities ? 1 : -1));
		if (static_cast<int>(Rarity) >= static_cast<int>(ERarity::Max) - 1)
		{
			HasTriedAboveRarities = true;
			Rarity = static_cast<ERarity>(static_cast<int>(ERarity::Max) - 1);
		}
	} while ((Choice == nullptr || !WeaponUpgrades.Contains(Rarity)) && !(static_cast<int>(Rarity) == 0 && HasTriedAboveRarities));

	return Choice;
}
