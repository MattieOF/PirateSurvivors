// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PirateGameModeBase.generated.h"

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
	
protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
