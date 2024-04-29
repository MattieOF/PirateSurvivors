// copyright lololol

#include "UI/HealthBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Core/HealthComponent.h"

void UHealthBar::Initialise(UHealthBars* InContainer, const FText& Name, UHealthComponent* HealthComponent)
{
	check(InContainer);
	check(HealthComponent);
	
	NameText->SetText(Name);
	ProgressBar->SetPercent(HealthComponent->GetHealthPercent());
	HealthComponent->OnHealthChanged.AddDynamic(this, &UHealthBar::OnHealthChanged);
	HealthComponent->OnDeath.AddDynamic(this, &UHealthBar::OnDeath);
	HealthComp = HealthComponent;
	Container = InContainer;
}

// Can't be const due to the dynamic delegate binding
// ReSharper disable once CppMemberFunctionMayBeConst
void UHealthBar::OnHealthChanged(float Change, float NewHP)
{
	ProgressBar->SetPercent(HealthComp->GetHealthPercent());
	BackBarCooldown = .5f;
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Animate backbar to match progress bar
	if (BackBarCooldown > 0)
		BackBarCooldown -= InDeltaTime;
	else if (BackBarCooldown <= 0)
		BackBar->SetPercent(FMath::FInterpTo(BackBar->GetPercent(), ProgressBar->GetPercent(), InDeltaTime, 5));
}
