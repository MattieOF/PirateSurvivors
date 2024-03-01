// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "GameFramework/Character.h"
#include "PirateSurvivorsCharacter.generated.h"

UCLASS()
class PIRATESURVIVORS_API APirateSurvivorsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APirateSurvivorsCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Survivors Character")
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pirate Survivors Character")
	UHealthComponent* HealthComponent;
};
