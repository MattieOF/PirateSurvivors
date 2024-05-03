// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PirateLobbyGameMode.generated.h"

UCLASS()
class PIRATESURVIVORS_API APirateLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

	APirateLobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
public:
	UFUNCTION(BlueprintCallable)
	void AddReadiedPlayer(class APiratePlayerState* PlayerState);

protected:
	UPROPERTY(BlueprintReadOnly)
	int ReadyPlayers = 0;

	UPROPERTY()
	TArray<class APiratePlayerState*> ReadiedPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
	FString MapToLoad = "/Game/Maps/MyMap?listen";
};
