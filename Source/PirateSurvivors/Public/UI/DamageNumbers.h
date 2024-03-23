// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumbers.generated.h"

class UCanvasPanel;
class UCanvasPanelSlot;
class UTextBlock;

struct FDmgNumber
{
	UTextBlock* Widget;
	FVector Loc;
	FVector2D EndOffset;
	UCanvasPanelSlot* Slot;
	float BaseLifetime, Lifetime;
};

/**
 * Should be implemented in blueprint
 */
UCLASS()
class PIRATESURVIVORS_API UDamageNumbers : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Damage Numbers")
	void AddDamageNumber(FVector WorldLoc, int Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Numbers")
	FLinearColor DmgColor = FLinearColor::MakeFromHSV8(0, 200, 255);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Numbers")
	FLinearColor HealColor = FLinearColor::Green;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* DamageNumbersCanvas;

	// TODO: Is this the best way to store this?
	// Removing the oldest damage number will result in a reallocation of the array since it's at the start?
	TArray<FDmgNumber> DamageNumbers;
};
