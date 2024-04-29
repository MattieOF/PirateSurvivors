// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PiratePlayerController.generated.h"

struct FXPInfo;
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
	
	UFUNCTION(Client, Reliable)
	void Client_InitialiseXP(const TArray<FXPInfo>& XPItems);

	UFUNCTION()
	void EnsureXPReplicated();
	
	UFUNCTION(Client, Reliable)
	void Client_CallCreateUI();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "UI")
	void CreateUI();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Player Controller")
	bool HasLoaded() const;
	
	UPROPERTY(BlueprintReadWrite)
	APiratePlayerCharacter* Pirate;
};
