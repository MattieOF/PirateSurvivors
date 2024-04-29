// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

// Forward declarations
class UHealthBars;
class UHealthComponent;
class UTextBlock;
class UProgressBar;

UCLASS()
class PIRATESURVIVORS_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Health Bar")
	void Initialise(UHealthBars* InContainer, const FText& Name, UHealthComponent* HealthComponent);
	
	UFUNCTION()
	void OnHealthChanged(float Change, float NewHP);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* BackBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText = nullptr;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Health Bar")
	UHealthComponent* HealthComp = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Health Bar")
	UHealthBars* Container = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Health Bar")
	float BackBarCooldown = 0;
};
