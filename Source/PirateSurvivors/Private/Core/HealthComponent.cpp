// copyright lololol

#include "Core/HealthComponent.h"

#include "PirateLog.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Only ever ticks on server
	if (bEnableOverheal)
	{
		if (MaxHealth < Health)
		{
			// We're overhealed, decay!!
			CurrentOverhealDecayDelay -= DeltaTime;
			if (CurrentOverhealDecayDelay <= 0)
			{
				SetHP(FMath::Max(MaxHealth, Health - OverhealDecayMagnitude), false);
				CurrentOverhealDecayDelay = OverhealDecayRate;
			}
		}
	}
}

void UHealthComponent::TakeDamage(const FDamageInstance& DamageEvent)
{
	if (IsDead())
		return;
	
	if (!GetOwner()->HasAuthority())
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Only the server can process damage through UHealthComponent::TakeDamage!");
		return;
	}
	
	FDamageInstance Event = DamageEvent; // Copy the event so we can modify it, needed for armour calculations
	float Armour = 0;
	if (ArmourGetter.IsBound())
		Armour = ArmourGetter.Execute(Event);
	Event.Damage = FMath::Max(0.f, Event.Damage - (Armour / 2)); // TODO: Refine this formula

	Multicast_TakeDamage(Event);
}

void UHealthComponent::Multicast_TakeDamage_Implementation(const FDamageInstance& DamageEvent)
{
	// if (GetOwner()->HasAuthority())
	ChangeHP(-DamageEvent.Damage);
	
	if (bRetainDamageEvents)
		DamageHistory.Add(DamageEvent);
}

void UHealthComponent::ChangeHP(const float Amount)
{
	SetHP(Health + Amount);
}

void UHealthComponent::Multicast_ChangeHP_Implementation(const float Amount)
{
	ChangeHP(Amount);
}

void UHealthComponent::SetHP(const float NewHP, const bool bResetPrevious)
{
	Health = NewHP;

	if (Health > MaxHealth)
	{
		if (!bEnableOverheal)
			Health = MaxHealth;
		else
		{
			PrimaryComponentTick.SetTickFunctionEnable(true);
		}
	}

	if (Health < 0 + KINDA_SMALL_NUMBER)
		Health = 0;

	if (Health == PreviousHP)
		return;
	if (Health < PreviousHP)
		OnHurt.Broadcast(PreviousHP - Health, Health);
	else
		OnHealed.Broadcast(Health - PreviousHP, Health);

	OnHealthChanged.Broadcast(Health - PreviousHP, Health);
	
	if (Health <= 0 && GetOwner()->HasAuthority())
		Multicast_Die();
	
	if (bResetPrevious)
		PreviousHP = Health;
}

void UHealthComponent::Multicast_SetHP(const float NewHP, const bool bResetPrevious)
{
	SetHP(NewHP, bResetPrevious);
}

void UHealthComponent::SetMaxHP(const float NewMaxHP, const bool bClampHP, const bool bGiveDifference)
{
	if (!GetOwner()->HasAuthority())
		return;

	const float Diff = NewMaxHP - MaxHealth;
	MaxHealth = NewMaxHP;
	OnHealthChanged.Broadcast(0, Health); // This will update the health bar, etc.
	if (bGiveDifference && !bDead)
		SetHP(Health + Diff);
	if (bClampHP)
		SetHP(FMath::Clamp(Health, 0.f, MaxHealth));
}

void UHealthComponent::Revive(float RevivalHPPercentage)
{
	bDead = false;
	Multicast_SetHP(RevivalHPPercentage * MaxHealth);
}

void UHealthComponent::Multicast_Die_Implementation()
{
	bDead = true;
	OnDeath.Broadcast();
	Health = 0;	
}

void UHealthComponent::SetOverhealEnabled(const bool bEnable, const bool bClampHP)
{
	if (GetOwner()->HasAuthority())
	{
		bEnableOverheal = bEnable;
		PrimaryComponentTick.SetTickFunctionEnable(bEnable);

		if (bClampHP)
			SetHP(FMath::Clamp(Health, 0.f, MaxHealth));
	}
}

void UHealthComponent::OnRep_Health(float OldHealth)
{
	PreviousHP = Health;
}

void UHealthComponent::OnRep_MaxHealth()
{
	OnHealthChanged.Broadcast(0, Health);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME_CONDITION(UHealthComponent, bRetainDamageEvents, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, bEnableOverheal, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, OverhealDecayRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UHealthComponent, OverhealDecayMagnitude, COND_OwnerOnly);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
		PrimaryComponentTick.SetTickFunctionEnable(bEnableOverheal);
	else
		PrimaryComponentTick.SetTickFunctionEnable(false);
}
