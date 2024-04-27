// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Upgrade.h"
#include "PirateGameModeBase.generated.h"

// Forward decls
class UUpgradeList;
class APiratePlayerState;
class UUpgrade;
class UEnemyData;
enum class ERarity : uint8;
class AXP;
class AXPManager;

/**
 * Base game mode class for Pirate Survivors
 */
UCLASS()
class PIRATESURVIVORS_API APirateGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APirateGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Game Settings")
	// TStaticArray<float, static_cast<int>(ERarity::Max)> RarityProbabilities;
	TArray<float> RarityProbabilities;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static FORCEINLINE APirateGameModeBase* GetPirateGameMode(const UObject* WorldContextObject)
	{
		return Cast<APirateGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	}

	UFUNCTION(Exec, Category = "Pirate Game Mode")
	void ListPlayerIndexes() const;

	UFUNCTION(Exec, Category = "Pirate Game Mode")
	void ListWeapons() const;

	UFUNCTION(Exec, Category = "Pirate Game Mode")
	void ListEnemies() const;
	
	UFUNCTION(Exec, BlueprintCallable, Category = "Pirate Game Mode")
	void GivePlayerWeapon(int PlayerIndex, FString WeaponName);

	UFUNCTION(BlueprintCallable, Category = "Pirate Game Mode")
	void SpawnEnemy(UEnemyData* EnemyType, const FVector& Location, const FRotator& Rotation);

	UFUNCTION(Exec, Category = "Pirate Game Mode")
	void SpawnEnemyNearby(const FString& EnemyType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Game Mode")
	virtual ERarity GetRarityForPlayer(APiratePlayerState* Player) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Game Mode")
	FORCEINLINE UUpgradeList* GetUpgradeList() const { return UpgradeList; }
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Pirate Game Mode")
	UUpgradeList* UpgradeList;

	UFUNCTION(BlueprintNativeEvent)
	void CreateUpgradeList();
	
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	float RarityProbabilitySum = 0;
};
