// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PiratePlayerController.generated.h"

class APiratePlayerCharacter;
/**
 * Player controller class for Pirate Survivors
 */
UCLASS()
class PIRATESURVIVORS_API APiratePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(BlueprintReadWrite)
	APiratePlayerCharacter* Pirate;
};
