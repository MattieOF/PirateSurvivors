// copyright lololol

#include "UI/DamageNumbers.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UDamageNumbers::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const auto LocalController = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld());
	
	for (int i = DamageNumbers.Num() - 1; i >= 0; i--)
	{
		auto& Dmg = DamageNumbers[i];
		
		Dmg.Lifetime -= InDeltaTime;
		if (Dmg.Lifetime <= 0)
		{
			Dmg.Widget->RemoveFromParent();
			DamageNumbers.RemoveAt(i);
			continue;
		}

		FVector2D Loc;
		if (!UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(LocalController, Dmg.Loc, Loc, false))
		{
			// We're offscreen, remove from screen
			Dmg.Widget->RemoveFromParent();
			DamageNumbers.RemoveAt(i);
			continue;
		}
		
		const float LifetimePercent = Dmg.Lifetime / Dmg.BaseLifetime;
		
		Dmg.Slot->SetPosition(FMath::Lerp(Loc, Loc + Dmg.EndOffset, 1 - LifetimePercent));
		
		float Scale = 1;
		if (LifetimePercent < 0.2f)
			Scale = FMath::Lerp(0.f, 1.f, LifetimePercent * 5);
		else if (LifetimePercent > 0.8f)
			Scale = FMath::Lerp(1.f, 0.f, (LifetimePercent - 0.8) * 5);
		Dmg.Widget->SetRenderScale(FVector2D(Scale));
	}
}

void UDamageNumbers::AddDamageNumber(FVector WorldLoc, int Damage)
{
	FVector2D Loc;
	const auto LocalController = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld());
	if (!UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(LocalController, WorldLoc, Loc, false))
	{
		// We're offscreen, don't bother
		return;	
	}
	
	UTextBlock* Text = NewObject<UTextBlock>();
	Text->SetText(FText::AsNumber(FMath::Abs(Damage)));
	if (Damage < 0)
		Text->SetColorAndOpacity(HealColor);
	else
		Text->SetColorAndOpacity(DmgColor);
	
	auto DmgNumSlot = DamageNumbersCanvas->AddChildToCanvas(Text);
	DmgNumSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	DmgNumSlot->SetPosition(Loc);

	FDmgNumber DmgNumber;
	DmgNumber.Widget = Text;
	DmgNumber.Loc = WorldLoc;
	DmgNumber.EndOffset = FVector2D(FMath::RandRange(-100, 100), FMath::RandRange(-75, 75));
	DmgNumber.Slot = DmgNumSlot;
	DmgNumber.BaseLifetime = 2.0f;
	DmgNumber.Lifetime = 2.0f;
	DamageNumbers.Add(DmgNumber);
}
