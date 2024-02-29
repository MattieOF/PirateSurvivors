// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerClassData.generated.h"

struct FPlayerStatUpgrade;
/**
 * Data relating to a class of player, such as base stats, abilities, etc.
 */
UCLASS()
class PIRATESURVIVORS_API UPlayerClassData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Class Info")
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Class Info")
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Class Info")
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Class Info")
	TArray<FPlayerStatUpgrade> BaseStatUpgrades;
};
