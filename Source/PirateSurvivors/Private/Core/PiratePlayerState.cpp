// copyright lololol

#include "Core/PiratePlayerState.h"

#include "PirateLog.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/WeaponData.h"
#include "Weapon/WeaponFunctionality.h"

void APiratePlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	// Init state
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
	while (AddedXP > 0)
	{
		const float XPMul = GetCurrentXPMultiplier();
		float AdjustedXP = AddedXP * XPMul;
		if (AdjustedXP > (1 - this->XP))
		{
			Level++;
			AdjustedXP -= 1 - this->XP;
			this->XP = 0;
			AddedXP = AdjustedXP / XPMul;
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

void APiratePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APiratePlayerState, Level);
	DOREPLIFETIME(APiratePlayerState, XP);
}
