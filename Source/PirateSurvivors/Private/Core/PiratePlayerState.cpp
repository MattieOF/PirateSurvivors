// copyright lololol

#include "Core/PiratePlayerState.h"

#include "PirateLog.h"
#include "Core/InteractableComponent.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerController.h"
#include "Core/UpgradeList.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/Upgrade.h"
#include "Weapon/WeaponData.h"
#include "Weapon/WeaponFunctionality.h"

void APiratePlayerState::BeginPlay()
{
	Super::BeginPlay();
	Initialise();
}

void APiratePlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (AWeaponFunctionality* Weapon : Weapons)
	{
		if (Weapon && Weapon->bRequestReposition)
			Weapon->SetActorTransform(GetPawn()->GetActorTransform());
	}
}

void APiratePlayerState::Initialise()
{
	// if (!GetPawn())
	// 	return; // ?????????????

	if (!GetPiratePawn())
	{
		GetWorldTimerManager().SetTimerForNextTick([this] { Initialise(); });
		return;
	}
	
	PlayerStats = NewObject<UPlayerStats>(this);

	GetPiratePawn()->GetHealthComponent()->SetMaxHP(PlayerStats->MaxHealth);
	GetPiratePawn()->GetCharacterMovement()->MaxWalkSpeed = PlayerStats->MaxSpeed;
	GetPiratePawn()->GetCharacterMovement()->JumpZVelocity = PlayerStats->JumpHeight;
	GetPiratePawn()->GetReviveInteraction()->Multicast_SetHoldTime(PlayerStats->TimeToRevive);
	
	PlayerStats->OnMaxHealthChanged.AddDynamic(this, &APiratePlayerState::OnMaxHealthChanged);
	PlayerStats->OnMaxSpeedChanged.AddDynamic(this, &APiratePlayerState::OnMaxSpeedChanged);
	PlayerStats->OnJumpHeightChanged.AddDynamic(this, &APiratePlayerState::OnJumpHeightChanged);
	PlayerStats->OnTimeToReviveChanged.AddDynamic(this, &APiratePlayerState::OnTimeToReviveChanged);
	
	AWeaponFunctionality* Null = nullptr;
	Weapons.Init(Null, BaseWeaponSlotCount);
	const APirateGameModeBase* GameMode = APirateGameModeBase::GetPirateGameMode(GetWorld());
	GiveWeaponFromType(GameMode->DefaultWeapon);
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
	// if (GetNetMode() == NM_Client)
	// {
	// 	PIRATE_LOG_ERROR("Attempted to add XP on client!");
	// 	return;
	// }
	
	while (AddedXP > 0)
	{
		const float XPMul = GetCurrentXPMultiplier();
		float AdjustedXP = AddedXP * XPMul;
		if (AdjustedXP + KINDA_SMALL_NUMBER > 1 - this->XP)
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

void APiratePlayerState::Client_ReceiveUpgradeChoices_Implementation(const TArray<FQueuedUpgradeChoice>& UpgradeChoices)
{
	// PIRATE_LOGC_NOLOC(GetWorld(), "Received upgrade choices!");
	UpgradeQueue.Enqueue(UpgradeChoices);
	OnUpgradeChoicesReceived.Broadcast(UpgradeChoices, this);
}

void APiratePlayerState::OnLevelUp_Implementation(int NewLevel)
{
	if (GetNetMode() == NM_Client)
		return;
	
	const APirateGameModeBase* GameMode = APirateGameModeBase::GetPirateGameMode(GetWorld());

	if (Level % 8 == 0)
	{
		// Give the player a choice of weapon upgrades
		auto WeaponChoices = GameMode->GetUpgradeList()->GetWeaponChoices(this);
		if (WeaponChoices.Num() != 0)
		{
			UpgradeQueue.Enqueue(WeaponChoices); // Enqueue the upgrade choices on the server
			Client_ReceiveUpgradeChoices(WeaponChoices); // And send them to the client
			return;
		}
	}

	auto UpgradeChoices = GameMode->GetUpgradeList()->GetPlayerUpgradeChoices(this);

	if (UpgradeChoices.Num() == 0)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "No upgrade choices found for player %s?? Not sending anything", *GetName());
		return;
	}
	
	UpgradeQueue.Enqueue(UpgradeChoices); // Enqueue the upgrade choices on the server
	Client_ReceiveUpgradeChoices(UpgradeChoices); // And send them to the client
}

void APiratePlayerState::Multicast_AddWeaponUpgrade_Implementation(const FQueuedUpgradeChoice& UpgradeChoice)
{
	UWeaponUpgrade* Upgrade = Cast<UWeaponUpgrade>(UpgradeChoice.Upgrade);

	// Get all valid weapons for this upgrade
	TArray<TTuple<int, AWeaponFunctionality*>> ValidWeapons;
	if (UpgradeChoice.TargetWeaponIndex != -1)
	{
		if (!Weapons.IsValidIndex(UpgradeChoice.TargetWeaponIndex))
		{
			PIRATE_LOGC_ERROR(
				GetWorld(),
				"In player state %s, FQueuedUpgradeChoice has invalid target weapon index: %i. For upgrade %ls.",
				*GetName(), UpgradeChoice.TargetWeaponIndex, *Upgrade->GetName());
			return;
		}
		if (Upgrade->IsValidForWeapon(Weapons[UpgradeChoice.TargetWeaponIndex]))
			ValidWeapons.Add(TTuple<int, AWeaponFunctionality*>(UpgradeChoice.TargetWeaponIndex, Weapons[UpgradeChoice.TargetWeaponIndex]));
		else
		{
			PIRATE_LOGC_ERROR(GetWorld(), "Target weapon index %i is not valid for upgrade %s", UpgradeChoice.TargetWeaponIndex, *Upgrade->GetName());
			return;
		}
	} else {
		for (int i = 0; i < Weapons.Num(); i++)
		{
			if (Upgrade->IsValidForWeapon(Weapons[i]))
				ValidWeapons.Add(TTuple<int, AWeaponFunctionality*>(i, Weapons[i]));
		}
	}

	// if (Upgrade->bForOneWeapon)
	// {
	// 	// If we only want to apply to one weapon, pick a random one
	// 	if (ValidWeapons.Num() > 0)
	// 	{
	// 		// TODO: Lots of code duplication here, could be refactored
	// 		const int ValidWeaponIndex = FMath::RandRange(0, ValidWeapons.Num() - 1);
	// 		AWeaponFunctionality* NewWeapon = Upgrade->ApplyUpgrade(this, ValidWeapons[ValidWeaponIndex].Value);
	// 		if (NewWeapon != ValidWeapons[ValidWeaponIndex].Value)
	// 		{
	// 			const int Index = ValidWeapons[ValidWeaponIndex].Key;
	// 			Weapons[Index] = NewWeapon;
	// 			OnWeaponUpdated.Broadcast(Index, NewWeapon);
	// 		}
	// 	} else
	// 	{
	// 		PIRATE_LOGC_WARN(GetWorld(), "No valid weapons for upgrade %s", *Upgrade->GetName());
	// 	}
	// } else
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

void APiratePlayerState::Multicast_AddStatUpgrade_Implementation(const FQueuedUpgradeChoice& Upgrade)
{
	const UPlayerUpgrade* PlayerUpgrade = Cast<UPlayerUpgrade>(Upgrade.Upgrade);
	if (PlayerUpgrade)
		PlayerUpgrade->ApplyUpgrade(this);
	else
		PIRATE_LOGC_ERROR(GetWorld(), "In player %s, upgrade %s is not a player upgrade, but was applied as one!", *GetName(), *Upgrade.Upgrade->GetName());
}

void APiratePlayerState::Server_SelectUpgrade_Implementation(int Index)
{
	if (GetPiratePawn()->IsDown())
		return;
	
	// Try and dequeue the current choices array
	TArray<FQueuedUpgradeChoice> UpgradeChoices;
	const bool DidFind = UpgradeQueue.Dequeue(UpgradeChoices);

	// If we didn't find any choices, log an error and return
	if (!DidFind)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "In player %s, attempted to select upgrade, but no upgrade choices were found!", *GetName());
		return;
	}

	// And if the provided index is invalid, log an error and return
	if (!UpgradeChoices.IsValidIndex(Index))
	{
		PIRATE_LOGC_ERROR(GetWorld(), "In player %s, attempted to select upgrade at index %i, but that index is invalid!", *GetName(), Index);
		return;
	}

	// Ok, we've got a valid choice, let's apply it
	// Make sure we call the correct function based on the type of upgrade
	const FQueuedUpgradeChoice Choice = UpgradeChoices[Index];
	if (Choice.Weapon != nullptr)
		GiveWeaponFromType(Choice.Weapon);
	else if (Choice.Upgrade->IsA(UWeaponUpgrade::StaticClass()))
		Multicast_AddWeaponUpgrade(Choice);
	else
		Multicast_AddStatUpgrade(Choice);
}

bool APiratePlayerState::IsUpgradeIndexValid(const int Index)
{
	return UpgradeQueue.Peek() != nullptr && UpgradeQueue.Peek()->IsValidIndex(Index);
}

void APiratePlayerState::SelectUpgrade(int Index)
{
	OnUpgradeChosen.Broadcast(Index);
	Server_SelectUpgrade(Index);
	UpgradeQueue.Pop();
}

bool APiratePlayerState::TryGetNextUpgradeChoices(TArray<FQueuedUpgradeChoice>& OutChoice)
{
	return UpgradeQueue.Peek(OutChoice);
}

void APiratePlayerState::SetInteractable(UInteractableComponent* NewInteractable)
{
	CurrentInteractable = NewInteractable;
	OnInteractableUpdated.Broadcast(CurrentInteractable);
}

void APiratePlayerState::OnMaxHealthChanged(float NewValue)
{
	GetPiratePawn()->GetHealthComponent()->SetMaxHP(NewValue, true, true);
}

void APiratePlayerState::OnMaxSpeedChanged(float NewValue)
{
	GetPiratePawn()->GetCharacterMovement()->MaxWalkSpeed = NewValue;
}

void APiratePlayerState::OnJumpHeightChanged(float NewValue)
{
	GetPiratePawn()->GetCharacterMovement()->JumpZVelocity = NewValue;
}

void APiratePlayerState::OnTimeToReviveChanged(float NewValue)
{
	GetPiratePawn()->GetReviveInteraction()->Multicast_SetHoldTime(NewValue);
}

void APiratePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APiratePlayerState, Level);
	DOREPLIFETIME(APiratePlayerState, XP);
}
