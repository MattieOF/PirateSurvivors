// copyright lololol

#include "Core/PiratePlayerState.h"

#include "PirateLog.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerController.h"
#include "Core/UpgradeList.h"
#include "Net/UnrealNetwork.h"
#include "Player/Upgrade.h"
#include "Weapon/WeaponData.h"
#include "Weapon/WeaponFunctionality.h"

void APiratePlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	// Init state
	PlayerStats = NewObject<UPlayerStats>();
	AWeaponFunctionality* Null = nullptr;
	Weapons.Init(Null, BaseWeaponSlotCount);
}

APiratePlayerCharacter* APiratePlayerState::GetPiratePawn() const
{
	return Cast<APiratePlayerCharacter>(GetPawn());
}

APiratePlayerController* APiratePlayerState::GetPirateController() const
{
	return Cast<APiratePlayerController>(GetPlayerController());
}

void APiratePlayerState::Client_OnReceivedWeapon_Implementation(AWeaponFunctionality* Weapon)
{
	OnWeaponObtained.Broadcast(Weapon);
}

void APiratePlayerState::AddXP(float AddedXP)
{
	if (GetNetMode() == NM_Client)
	{
		PIRATE_LOG_ERROR("Attempted to add XP on client!");
		return;
	}
	
	while (AddedXP > 0)
	{
		const float XPMul = GetCurrentXPMultiplier();
		float AdjustedXP = AddedXP * XPMul;
		if (AdjustedXP > (1 - this->XP))
		{
			// Level up!
			Level++;
			AdjustedXP -= 1 - this->XP;
			this->XP = 0;
			AddedXP = AdjustedXP / XPMul;
			OnLevelUp(Level);
		} else
		{
			this->XP += AdjustedXP;
			AddedXP = 0;
		}
	}

	OnXPUpdated.Broadcast(XP, Level);
}

bool APiratePlayerState::GiveWeaponFromType(UWeaponData* Weapon)
{
	if (!HasAuthority())
	{
		PIRATE_LOG_ERROR("Attempted to give weapon from functionality actor on client!");
		return false;
	}
	
	for (int i = 0; i < Weapons.Num(); i++)
	{
		if (!Weapons[i])
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetPirateController();
			AWeaponFunctionality* NewWeapon = GetWorld()->SpawnActor<AWeaponFunctionality>(Weapon->WeaponFunctionalitySubclass, SpawnParams);
			NewWeapon->Initialise(GetPiratePawn(), Weapon);
			Weapons[i] = NewWeapon;
			// Client_OnReceivedWeapon(NewWeapon);
			return true;
		}
	}
	
	return false;
}

bool APiratePlayerState::GiveWeaponFromFunctionalityActor(AWeaponFunctionality* Weapon)
{
	if (!HasAuthority())
	{
		PIRATE_LOG_ERROR("Attempted to give weapon from functionality actor on client!");
		return false;
	}
		
	for (int i = 0; i < Weapons.Num(); i++)
	{
		if (!Weapons[i])
		{
			Weapons[i] = Weapon;
			// Client_OnReceivedWeapon(Weapon);
			return true;
		}
	}
	
	return false;
}

void APiratePlayerState::OnRep_Level() const
{
	OnXPUpdated.Broadcast(XP, Level);
}

void APiratePlayerState::OnRep_XP() const
{
	OnXPUpdated.Broadcast(XP, Level);
}

void APiratePlayerState::OnLevelUp_Implementation(int NewLevel)
{
	const APirateGameModeBase* GameMode = APirateGameModeBase::GetPirateGameMode(GetWorld());
	auto UpgradeChoices = GameMode->GetUpgradeList()->GetPlayerUpgradeChoices(this);
	PIRATE_LOGC(GetWorld(), "Level up! Upgrade choices:");
	for (const auto& Upgrade : UpgradeChoices)
		PIRATE_LOGC_NOLOC(GetWorld(), "    %s", *Upgrade->GetName());
}

void APiratePlayerState::Multicast_AddWeaponUpgrade_Implementation(UWeaponUpgrade* Upgrade)
{
	// Get all valid weapons for this upgrade
	TArray<TTuple<int, AWeaponFunctionality*>> ValidWeapons;
	for (int i = 0; i < Weapons.Num(); i++)
	{
		if (Upgrade->IsValidForWeapon(Weapons[i]))
			ValidWeapons.Add(TTuple<int, AWeaponFunctionality*>(i, Weapons[i]));
	}

	if (Upgrade->bForOneWeapon)
	{
		// If we only want to apply to one weapon, pick a random one
		if (ValidWeapons.Num() > 0)
		{
			// TODO: Lots of code duplication here, could be refactored
			const int ValidWeaponIndex = FMath::RandRange(0, ValidWeapons.Num() - 1);
			AWeaponFunctionality* NewWeapon = Upgrade->ApplyUpgrade(this, ValidWeapons[ValidWeaponIndex].Value);
			if (NewWeapon != ValidWeapons[ValidWeaponIndex].Value)
			{
				const int Index = ValidWeapons[ValidWeaponIndex].Key;
				Weapons[Index] = NewWeapon;
				OnWeaponUpdated.Broadcast(Index, NewWeapon);
			}
		} else
		{
			PIRATE_LOGC_WARN(GetWorld(), "No valid weapons for upgrade %s", *Upgrade->GetName());
		}
	} else
	{
		for (int i = 0; i < ValidWeapons.Num(); i++)
		{
			AWeaponFunctionality* NewWeapon = Upgrade->ApplyUpgrade(this, ValidWeapons[i].Value);
			if (NewWeapon != ValidWeapons[i].Value)
			{
				const int Index = ValidWeapons[i].Key;
				Weapons[Index] = NewWeapon;
				OnWeaponUpdated.Broadcast(Index, NewWeapon);
			}
		}
	}
}

void APiratePlayerState::Multicast_AddStatUpgrade_Implementation(UPlayerUpgrade* Upgrade)
{
	Upgrade->ApplyUpgrade(this);
}

void APiratePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APiratePlayerState, Level);
	DOREPLIFETIME(APiratePlayerState, XP);
}
