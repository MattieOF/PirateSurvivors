// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PirateGameInstance.generated.h"

// Forward decls
class UDamageNumbers;
class UEnemyData;
class UWeaponData;
struct FXPInfo;

/**
 * Game Instance class for Pirate Survivors.
 * Holds data that persists throughout the entire session, including static settings.
 */
UCLASS()
class PIRATESURVIVORS_API UPirateGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Game Instance", meta = (WorldContext = "WorldContextObject"))
	static FORCEINLINE UPirateGameInstance* GetPirateGameInstance(UObject* WorldContextObject)
	{
		return Cast<UPirateGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Game Instance")
	FORCEINLINE TMap<FString, UWeaponData*> GetWeapons() const { return Weapons; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Game Instance")
	FORCEINLINE UWeaponData* GetWeapon(const FString& WeaponName) const { return Weapons.Contains(WeaponName) ? Weapons[WeaponName] : nullptr; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Game Instance")
	FORCEINLINE TMap<FString, UEnemyData*> GetEnemies() const { return Enemies; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Game Instance")
	FORCEINLINE UEnemyData* GetEnemy(const FString& EnemyName) const { return Enemies.Contains(EnemyName) ? Enemies[EnemyName] : nullptr; }
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FXPInfo> XPToBeReplicated;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, UWeaponData*> Weapons;
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, UEnemyData*> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pirate Game Instance")
	TSubclassOf<UUserWidget> DamageNumbersClass;
};
