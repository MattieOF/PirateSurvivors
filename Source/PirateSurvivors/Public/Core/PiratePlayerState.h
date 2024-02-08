// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "PirateGameState.h"
#include "GameFramework/PlayerState.h"
#include "PiratePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXPUpdated, float, NewXP, int, NewLevel);

/**
 * Base player state class for Pirate Survivors
 */
UCLASS()
class PIRATESURVIVORS_API APiratePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere, Category = "Pirate Player State", ReplicatedUsing = OnRep_Level)
	int Level = 0;
	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere, Category = "Pirate Player State", ReplicatedUsing = OnRep_XP)
	float XP = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Player State")
	FORCEINLINE float GetCurrentXPMultiplier() const
	{
		const APirateGameState* const GameState = APirateGameState::GetPirateGameState(this);
		return Level < GameState->XPMultipliers.Num() ? GameState->XPMultipliers[Level] : GameState->XPMultipliers.Last();
	}

	UFUNCTION(BlueprintCallable, Category = "Pirate Player State")
	void AddXP(float AddedXP);

	UPROPERTY(BlueprintAssignable, Category = "Pirate Player State")
	FOnXPUpdated OnXPUpdated;

	UFUNCTION()
	FORCEINLINE void OnRep_Level() const;
	UFUNCTION()
	FORCEINLINE void OnRep_XP() const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
