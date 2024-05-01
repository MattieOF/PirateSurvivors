// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EnemyStatics.generated.h"

// Forward decls
class APiratePlayerCharacter;

UCLASS()
class PIRATESURVIVORS_API UEnemyStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Enemy Statics", meta = (WorldContext = "WorldContextObject"))
	static APiratePlayerCharacter* GetClosestPlayer(UObject* WorldContextObject, const FVector& Location, float& OutDistance, bool bIncludeDead = false);

	UFUNCTION(BlueprintCallable, Category = "Enemy Statics", meta = (WorldContext = "WorldContextObject"))
	static FORCEINLINE APiratePlayerCharacter* GetClosestPlayerNoDist(UObject* WorldContextObject, const FVector& Location, bool bIncludeDead = false)
	{
		float _;
		return GetClosestPlayer(WorldContextObject, Location, _, bIncludeDead);
	}
};
