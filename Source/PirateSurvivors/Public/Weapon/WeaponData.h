// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

class UWeaponStats;

/**
 * This class stores the base values of properties all weapons have, such as name, damage, fire rate, etc.
 */
UCLASS()
class PIRATESURVIVORS_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TArray<FName> Tags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UWeaponStats> WeaponStatsSubclass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class AWeaponFunctionality> WeaponFunctionalitySubclass;

    UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Weapon")
	UWeaponStats* BaseWeaponStats;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool VerifyCompatability() const;

	UFUNCTION(CallInEditor, Category = "Weapon")
	FORCEINLINE void CheckCompatibility() const { VerifyCompatability(); };
};
