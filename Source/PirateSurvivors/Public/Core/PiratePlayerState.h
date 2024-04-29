// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "PirateGameState.h"
#include "GameFramework/PlayerState.h"
#include "Player/PlayerStats.h"
#include "PiratePlayerState.generated.h"

// Forward decls
struct FQueuedUpgradeChoice;
class UUpgrade;
class UWeaponUpgrade;
class UPlayerUpgrade;
class APiratePlayerController;
class APiratePlayerCharacter;
class UWeaponData;
class AWeaponFunctionality;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXPUpdated, float, NewXP, int, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponObtained, AWeaponFunctionality*, NewWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponUpdated, int, Slot, AWeaponFunctionality*, NewWeapon);

// UHT doesn't throw an error about it, but TArrays as parameters in delegates must be const references, or assigning in blueprints will not work.
// Thanks to https://forums.unrealengine.com/t/signature-error-the-function-event-does-not-match-the-necessary-signature-has-the-delegate-or-function-event-changed/724030/6

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeChoicesReceived, const TArray<FQueuedUpgradeChoice>&, Choices, APiratePlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeChosen, int, UpgradeIndex);

/**
 * Base player state class for Pirate Survivors
 */
UCLASS()
class PIRATESURVIVORS_API APiratePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void Initialise();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Player State")
	FORCEINLINE float GetCurrentXPMultiplier() const
	{
		const APirateGameState* const GameState = APirateGameState::GetPirateGameState(this);
		if (!GameState)
			return 0;
		return Level < GameState->XPMultipliers.Num() ? GameState->XPMultipliers[Level] : GameState->XPMultipliers.Last();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Player State")
	APiratePlayerCharacter* GetPiratePawn() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Player State")
	APiratePlayerController* GetPirateController() const;

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Pirate Player State")
	void Client_OnReceivedWeapon(AWeaponFunctionality* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Pirate Player State")
	void AddXP(float AddedXP);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Player State")
	FORCEINLINE bool HasFreeWeaponSlot() { for (const AWeaponFunctionality* Weapon : Weapons) { if (Weapon == nullptr) return true; } return false; };
	
	UFUNCTION(BlueprintCallable, Category = "Pirate Player State")
	bool GiveWeaponFromType(UWeaponData* Weapon);
	
	UFUNCTION(BlueprintCallable, Category = "Pirate Player State")
	bool GiveWeaponFromFunctionalityActor(AWeaponFunctionality* Weapon);

	UPROPERTY(BlueprintAssignable, Category = "Pirate Player State")
	FOnXPUpdated OnXPUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Pirate Player State")
	FOnWeaponObtained OnWeaponObtained;

	UPROPERTY(BlueprintAssignable, Category = "Pirate Player State")
	FOnWeaponUpdated OnWeaponUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Pirate Player State")
	FOnUpgradeChoicesReceived OnUpgradeChoicesReceived;

	UPROPERTY(BlueprintAssignable, Category = "Pirate Player State")
	FOnUpgradeChosen OnUpgradeChosen;
	
	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere, Category = "Pirate Player State", ReplicatedUsing = OnRep_Level)
	int Level = 0;
	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere, Category = "Pirate Player State", ReplicatedUsing = OnRep_XP)
	float XP = 0;

	UFUNCTION()
	void OnRep_Level() const;
	UFUNCTION()
	void OnRep_XP() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnLevelUp(int NewLevel);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pirate Player State")
	uint8 BaseWeaponSlotCount = 4;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_AddStatUpgrade(const FQueuedUpgradeChoice& Upgrade);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_AddWeaponUpgrade(const FQueuedUpgradeChoice& Upgrade);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_ReceiveUpgradeChoices(const TArray<FQueuedUpgradeChoice>& UpgradeChoices);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pirate Player State") // Replicated with RPCs
	UPlayerStats* PlayerStats;

	UPROPERTY(BlueprintReadWrite, Category = "Pirate Player State")
	TArray<UUpgrade*> AppliedUpgrades;

	const TArray<AWeaponFunctionality*>* GetWeapons() const { return &Weapons; }

	UFUNCTION(BlueprintCallable, Category = "Pirate Player State")
	void SelectUpgrade(int Index);
	
	UFUNCTION(BlueprintCallable, Category = "Pirate Player State", Server, Reliable)
	void Server_SelectUpgrade(int Index);
	
	UFUNCTION(BlueprintCallable, Category = "Pirate Player State")
	bool TryGetNextUpgradeChoices(TArray<FQueuedUpgradeChoice>& OutChoice);
	
	UFUNCTION()
	void OnMaxHealthChanged(float NewValue);
	
	UFUNCTION()
	void OnMaxSpeedChanged(float NewValue);
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Weapons")
	TArray<AWeaponFunctionality*> Weapons;

	TQueue<TArray<FQueuedUpgradeChoice>> UpgradeQueue;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
