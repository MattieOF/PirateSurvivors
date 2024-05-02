// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "PirateGameState.generated.h"

// Forward decls
class AEnemy;
class UHealthBars;
class UDamageNumbers;
class AXP;
class AXPManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossSpawned, AEnemy*, Enemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossKilled, AEnemy*, Enemy);

/**
 * Game	State class for Pirate Survivors.
 */
UCLASS()
class PIRATESURVIVORS_API APirateGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static FORCEINLINE APirateGameState* GetPirateGameState(const UObject* WorldContextObject)
	{
		return Cast<APirateGameState>(UGameplayStatics::GetGameState(WorldContextObject));
	}
	
	FORCEINLINE AXPManager* GetXPManager() const { return XPManager; }
	FORCEINLINE UDamageNumbers* GetDamageNumbers() const { return DamageNumbers; }
	FORCEINLINE UHealthBars* GetHealthBars() const { return HealthBars; }

	UFUNCTION(BlueprintCallable)
	void OnPlayerDied();
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_GameOver();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset References")
	TSubclassOf<AXP> XPClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<double> XPMultipliers = {1.0, 0.7, 0.7, 0.5, 0.5, 0.3, 0.3, 0.1, 0.1, 0.1, 0.05};

	UPROPERTY(BlueprintAssignable)
	FOnGameOver OnGameOver;

	UPROPERTY(BlueprintAssignable)
	FOnBossSpawned OnBossSpawned;

	UPROPERTY(BlueprintAssignable)
	FOnBossKilled OnBossKilled;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	AXPManager* XPManager;

	UPROPERTY(BlueprintReadOnly)
	UDamageNumbers* DamageNumbers;

	UPROPERTY(BlueprintReadOnly)
	UHealthBars* HealthBars;
};
