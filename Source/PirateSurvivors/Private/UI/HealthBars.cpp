// copyright lololol

#include "UI/HealthBars.h"

#include "PirateLog.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Core/HealthComponent.h"
#include "UI/HealthBar.h"

UHealthBar* UHealthBars::AddHealthBar(FText Name, UHealthComponent* HealthComponent)
{
	if (!HealthComponent)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Health component is null!");
		return nullptr;
	}

	if (!HealthComponent->GetOwner())
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Health component owner is null!");
		return nullptr;
	}

	if (!HealthBarClass)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Health bar class is null!");
		return nullptr;
	}
	
	const TSubclassOf<UUserWidget> ClassAsUserWidget { HealthBarClass };
	UHealthBar* HealthBar = CreateWidget<UHealthBar>(GetWorld(), ClassAsUserWidget);
	if (!HealthBar)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Failed to create health bar widget!");
		return nullptr;
	}
	HealthBar->Initialise(this, Name, HealthComponent);

	const auto HealthBarSlot = Canvas->AddChildToCanvas(HealthBar);
	HealthBarSlot->SetAlignment(FVector2D(0.5f, 1.f));
	HealthBarSlot->SetAutoSize(true);
	HealthBars.Add(HealthComponent, { HealthBar, FVector::ZeroVector });
	
	return HealthBar;
}

void UHealthBars::RemoveHealthBar(UHealthComponent* HealthComponent, bool bCallOnDeath)
{
	if (!HealthComponent)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Health component is null!");
		return;
	}

	if (!HealthBars.Contains(HealthComponent))
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Health component not found in health bars map!");
		return;
	}

	const auto& HealthBar = HealthBars[HealthComponent];
	if (bCallOnDeath)
		HealthBar.HealthBar->OnDeath();
	else
	{
		Residuals.Add({HealthBar.HealthBar, HealthBar.LastLocation});
		HealthBars.Remove(HealthComponent);
	}
}

void UHealthBars::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	const auto LocalController = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld());

	for (auto& HealthBar : HealthBars)
	{
		FVector2D Loc;
		HealthBar.Value.LastLocation = GetTargetLocation(HealthBar.Key);
		if (!UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(LocalController, HealthBar.Value.LastLocation, Loc, false))
			HealthBar.Value.HealthBar->SetVisibility(ESlateVisibility::Hidden);
		else
			HealthBar.Value.HealthBar->SetVisibility(ESlateVisibility::HitTestInvisible);
		Cast<UCanvasPanelSlot>(HealthBar.Value.HealthBar->Slot)->SetPosition(Loc);
	}

	for (int i = Residuals.Num() - 1; i >= 0; --i)
	{
		if (!Residuals[i].Key || !Residuals[i].Key->GetParent())
		{
			Residuals.RemoveAt(i);
			continue;
		}
		
		FVector2D Loc;
		if (!UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(LocalController, Residuals[i].Value, Loc, false))
		{
			Residuals[i].Key->RemoveFromParent();
			Residuals.RemoveAt(i);
			continue;
		}
		Cast<UCanvasPanelSlot>(Residuals[i].Key->Slot)->SetPosition(Loc);
	}
}

FVector UHealthBars::GetTargetLocation(const UHealthComponent* HealthComponent)
{
	FVector Location, Bounds;
	HealthComponent->GetOwner()->GetActorBounds(true, Location, Bounds);
	return Location + FVector(0, 0, Bounds.Z + 40);
}
