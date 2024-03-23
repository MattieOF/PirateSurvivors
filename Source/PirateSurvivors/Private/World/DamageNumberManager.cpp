// copyright lololol

#include "World/DamageNumberManager.h"

#include "PirateSurvivors.h"
#include "Components/WidgetComponent.h"
#include "Core/PirateGameInstance.h"
#include "UI/DamageNumberWidget.h"

DECLARE_CYCLE_STAT(TEXT("Damage Number Manager Update"), STAT_UpdateDmgNums, STATGROUP_PirateUI);

ADamageNumberManager::ADamageNumberManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADamageNumberManager::BeginPlay()
{
	Super::BeginPlay();
	if (!WidgetClass)
		WidgetClass = UPirateGameInstance::GetPirateGameInstance(GetWorld())->DamageNumberWidgetClass;
}

void ADamageNumberManager::Tick(float DeltaSeconds)
{
	SCOPE_CYCLE_COUNTER(STAT_UpdateDmgNums);
	
	Super::Tick(DeltaSeconds);

	// Iterate backwards so we can remove elements
	for (int i = DamageNumbers.Num() - 1; i >= 0; --i)
	{
		FDamageNumber& DamageNumber = DamageNumbers[i];
		
		DamageNumber.Lifetime -= DeltaSeconds;
		if (DamageNumber.Lifetime <= 0)
		{
			DamageNumber.WidgetComponent->DestroyComponent();
			DamageNumbers.RemoveAt(i);
			continue;
		}
		
		DamageNumber.WidgetComponent->AddWorldOffset(DamageNumber.Velocity * DeltaSeconds);
		DamageNumber.WidgetComponent->GetUserWidgetObject()->SetRenderScale(FVector2D(1, 1) * (DamageNumber.Lifetime / DamageNumber.BaseLifetime));
		DamageNumber.Velocity -= DamageNumber.Velocity * (DeltaSeconds * 2);
	}
}

void ADamageNumberManager::AddDamageNumber(int Damage, FVector Location)
{
	// No need to spawn damage numbers on a dedicated server
	if (GetNetMode() == NM_DedicatedServer)
		return;
	
	UWidgetComponent* WidgetComponent = NewObject<UWidgetComponent>(this);
	WidgetComponent->SetTickMode(ETickMode::Disabled);
	WidgetComponent->SetCastShadow(false);
	WidgetComponent->RegisterComponent();
	WidgetComponent->SetWidgetClass(WidgetClass);
	WidgetComponent->InitWidget();
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2D(100, 100));
	WidgetComponent->SetWorldLocation(Location);
	WidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (UDamageNumberWidget* DamageNumberWidget = Cast<UDamageNumberWidget>(WidgetComponent->GetUserWidgetObject()))
	{
		DamageNumberWidget->SetDamage(Damage);
	}

	FDamageNumber DamageNumber;
	DamageNumber.WidgetComponent = WidgetComponent;
	DamageNumber.Velocity = FVector(FMath::FRandRange(-50.f, 50.f), FMath::FRandRange(-50.f, 50.f), FMath::FRandRange(20.f, 60.f));
	DamageNumber.BaseLifetime = 2.0f;
	DamageNumber.Lifetime = 2.0f;
	DamageNumbers.Add(DamageNumber);
}
