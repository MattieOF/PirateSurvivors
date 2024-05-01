// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PSStatics.generated.h"

UCLASS()
class PIRATESURVIVORS_API UPSStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pirate Survivors Statics")
	static bool InActualGame();
};
