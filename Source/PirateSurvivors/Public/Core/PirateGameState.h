// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "PirateGameState.generated.h"

class AXP;
class AXPManager;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset References")
	TSubclassOf<AXP> XPClass;

protected:
	UPROPERTY(BlueprintReadOnly)
	AXPManager* XPManager;
};
