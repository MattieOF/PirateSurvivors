// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PirateGameModeBase.generated.h"

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

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static FORCEINLINE APirateGameModeBase* GetPirateGameMode(const UObject* WorldContextObject)
	{
		return Cast<APirateGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<double> XPMultipliers = {1.0, 0.7, 0.7, 0.5, 0.5, 0.3, 0.3, 0.1, 0.1, 0.1, 0.05};
	
protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
