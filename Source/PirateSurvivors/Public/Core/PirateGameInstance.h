// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PirateGameInstance.generated.h"

/**
 * Game Instance class for Pirate Survivors.
 * Holds data that persists throughout the entire session, including static settings.
 */
UCLASS()
class PIRATESURVIVORS_API UPirateGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Game Instance", meta = (WorldContext = "WorldContextObject"))
	static FORCEINLINE UPirateGameInstance* GetPirateGameInstance(UObject* WorldContextObject)
	{
		return Cast<UPirateGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	}
};
