// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberWidget.generated.h"

/**
 * Should be implemented by a widget blueprint.
 */
UCLASS()
class PIRATESURVIVORS_API UDamageNumberWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetDamage(int Damage);
};
