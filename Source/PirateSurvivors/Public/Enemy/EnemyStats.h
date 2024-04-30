// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Core/Stats.h"
#include "UObject/Object.h"
#include "EnemyStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyStatChanged, float, NewValue);

UCLASS(EditInlineNew, BlueprintType, Blueprintable, HideCategories = (Object))
class PIRATESURVIVORS_API UEnemyStats : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats", Setter = SetWalkSpeed)
	float WalkSpeed = 300.0f;
	UFUNCTION(BlueprintCallable)
	void SetWalkSpeed(float Value);
	UPROPERTY(BlueprintAssignable)
	FOnEnemyStatChanged OnWalkSpeedChanged;
};
