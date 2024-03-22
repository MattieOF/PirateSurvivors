// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PirateGameModeBase.generated.h"

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
	TMap<float, ERarity> RarityProbabilities;
	
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
	void SpawnEnemyNearby(FString EnemyType);
	
protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
