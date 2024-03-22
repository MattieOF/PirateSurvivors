// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "PirateGameState.h"
#include "GameFramework/PlayerState.h"
#include "PiratePlayerState.generated.h"

class APiratePlayerController;
// Forward decls
class APiratePlayerCharacter;
class UWeaponData;
class AWeaponFunctionality;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXPUpdated, float, NewXP, int, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponObtained, AWeaponFunctionality*, NewWeapon);

/**
 * Base player state class for Pirate Survivors
 */
UCLASS()
class PIRATESURVIVORS_API APiratePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Player State")
	FORCEINLINE float GetCurrentXPMultiplier() const
	{
		const APirateGameState* const GameState = APirateGameState::GetPirateGameState(this);
		return Level < GameState->XPMultipliers.Num() ? GameState->XPMultipliers[Level] : GameState->XPMultipliers.Last();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Player State")
	FORCEINLINE APiratePlayerCharacter* GetPiratePawn() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Player State")
	FORCEINLINE APiratePlayerController* GetPirateController() const;

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
	
	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere, Category = "Pirate Player State", ReplicatedUsing = OnRep_Level)
	int Level = 0;
	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere, Category = "Pirate Player State", ReplicatedUsing = OnRep_XP)
	float XP = 0;

	UFUNCTION()
	FORCEINLINE void OnRep_Level() const;
	UFUNCTION()
	FORCEINLINE void OnRep_XP() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pirate Player State")
	uint8 BaseWeaponSlotCount = 4;
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Weapons")
	TArray<AWeaponFunctionality*> Weapons;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
