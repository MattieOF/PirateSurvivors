// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBars.generated.h"

// Forward declarations
class UHealthComponent;
class UHealthBar;
class UCanvasPanel;

USTRUCT()
struct FHealthbarData
{
	GENERATED_BODY()

	UPROPERTY()
	UHealthBar* HealthBar;
	UPROPERTY()
	FVector LastLocation;
};

UCLASS()
class PIRATESURVIVORS_API UHealthBars : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Health Bars")
	UHealthBar* AddHealthBar(FText Name, UHealthComponent* HealthComponent);

	UFUNCTION(BlueprintCallable, Category = "Health Bars")
	void RemoveHealthBar(UHealthComponent* HealthComponent, bool bCallOnDeath = true);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Bars")
	TSubclassOf<UHealthBar> HealthBarClass;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	static FVector GetTargetLocation(const UHealthComponent* HealthComponent);
	
	UPROPERTY()
	TMap<UHealthComponent*, FHealthbarData> HealthBars;

	TArray<TTuple<UHealthBar*, FVector>> Residuals;
};
