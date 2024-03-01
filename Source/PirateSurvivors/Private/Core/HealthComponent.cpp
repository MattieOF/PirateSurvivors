// copyright lololol

#include "Core/HealthComponent.h"

#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEnableOverheal)
	{
		if (MaxHealth < Health)
		{
			// We're overhealed, decay!!
			// TODO: Should we call any delegates here?
			CurrentOverhealDecayDelay -= DeltaTime;
			if (CurrentOverhealDecayDelay <= 0)
			{
				Health = FMath::Max(MaxHealth, Health - OverhealDecayMagnitude);
				CurrentOverhealDecayDelay = OverhealDecayRate;
			}
		}
	}
}

void UHealthComponent::ChangeHP(const float Amount)
{
	SetHP(Health + Amount);
}

void UHealthComponent::SetHP(const float NewHP)
{
	const float OldHP = Health;
	Health = NewHP;

	if (Health > MaxHealth)
	{
		if (bEnableOverheal)
			Health = MaxHealth;
		else
		{
			PrimaryComponentTick.SetTickFunctionEnable(true);
		}
	}

	if (Health < OldHP)
		OnHurt.Broadcast(OldHP - Health, Health);
	else
		OnHealed.Broadcast(Health - OldHP, Health);

	OnHealthChanged.Broadcast(Health - OldHP, Health);
	
	if (Health <= 0)
	{
		OnDeath.Broadcast();
	}
}

void UHealthComponent::Hurt(float Damage)
{
	ChangeHP(-Damage);
}

void UHealthComponent::Heal(const float Amount)
{
	ChangeHP(Amount);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME()
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
		PrimaryComponentTick.SetTickFunctionEnable(bEnableOverheal);
	else
		PrimaryComponentTick.SetTickFunctionEnable(false);
}
