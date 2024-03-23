// copyright lololol

#include "Core/PirateGameState.h"

#include "PirateLog.h"
#include "Blueprint/UserWidget.h"
#include "Core/PirateGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "UI/DamageNumbers.h"
#include "World/XPManager.h"

void APirateGameState::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
		XPManager = GetWorld()->SpawnActor<AXPManager>();

	auto GI = UPirateGameInstance::GetPirateGameInstance(GetWorld());
	DamageNumbers = Cast<UDamageNumbers>(CreateWidget(GI, GI->DamageNumbersClass));
	if (!DamageNumbers)
	{
		PIRATE_LOG_ERROR(FString::Printf(TEXT("GI DamageNumbersClass is either unset or not a subclass of UDamageNumbers")));
	} else
	{
		DamageNumbers->AddToViewport(-100);
	}
}

void APirateGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APirateGameState, XPManager);
}
