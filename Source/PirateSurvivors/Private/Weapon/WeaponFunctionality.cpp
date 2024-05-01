// copyright lololol

#include "Weapon/WeaponFunctionality.h"

#include "Core/PiratePlayerState.h"
#include "Enemy/Enemy.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Projectile.h"
#include "Weapon/ProjectileData.h"
#include "Weapon/WeaponData.h"
#include "Weapon/WeaponStats.h"

class APiratePlayerState;

AWeaponFunctionality::AWeaponFunctionality()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetUpdateFrequency = 20; // Still a lot, but 1/5th of the default.
}

void AWeaponFunctionality::OnFire_Implementation()
{ }

void AWeaponFunctionality::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.SetTickFunctionEnable(true);

	if (WeaponData)
		Initialise(nullptr, WeaponData);
}

void AWeaponFunctionality::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!(HasAuthority() || GetLocalRole() == ROLE_AutonomousProxy))
		return;

	if (ReloadTime > 0)
	{
		ReloadTime -= DeltaSeconds;
		if (ReloadTime <= 0)
			Ammo = WeaponStats->Ammo;
	} else
	{
		// Don't try and fire (or do cooldown checks) if we're not the server.
		if (HasAuthority())
		{
			CurrentFireTime -= DeltaSeconds;
			if (CurrentFireTime <= 0)
			{
				if (!OwningCharacter->GetHealthComponent()->IsDead())
					OnFire();
				CurrentFireTime = WeaponStats->FireRateSeconds;
			}
		}
	}
}

void AWeaponFunctionality::Initialise(APirateSurvivorsCharacter* NewOwner, UWeaponData* Data)
{
	InitialiseLight(NewOwner, Data);
	WeaponStats = DuplicateObject<UWeaponStats>(Data->BaseWeaponStats, this);
	WeaponStats->SetFlags(WeaponStats->GetFlags() | RF_Public); // Thanks https://forums.unrealengine.com/t/uobject-eligible-for-replication-guide/671679/2
	Ammo = WeaponStats->Ammo; // Start with full ammo.

	// Dirty hack: if we're the server (or sp), and the local player, tell the player state we have this weapon, as
	// PostNetInit() won't be called (since we're the server).
	if (GetNetMode() == NM_ListenServer || GetNetMode() == NM_Standalone)
	{
		// @copypasta AWeaponFunctionality::PostNetInit
		const auto LocalController = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld());
		if (IsOwnedBy(LocalController))
			LocalController->GetPlayerState<APiratePlayerState>()->Client_OnReceivedWeapon(this);
	}

	OnInitialise();
}

void AWeaponFunctionality::InitialiseLight(APirateSurvivorsCharacter* NewOwner, UWeaponData* Data)
{
	WeaponData = Data;
	if (NewOwner)
		OwningCharacter = NewOwner;
}

void AWeaponFunctionality::OnInitialise_Implementation()
{
}

float AWeaponFunctionality::GetReloadProgress() const
{
	return FMath::Clamp(1 - (ReloadTime / WeaponStats->ReloadTime), 0.0f, 1.0f);
}

float AWeaponFunctionality::GetAmmoPercentage() const
{
	return FMath::Clamp(Ammo / WeaponStats->Ammo, 0.0f, 1.0f);
}

bool AWeaponFunctionality::UseAmmo(const float Amount)
{
	Ammo -= Amount;
	if (Ammo <= 0)
	{
		Ammo = 0;
		OnWeaponAmmoEmpty.Broadcast();
		ReloadTime = WeaponStats->ReloadTime; 
		return false;
	} else return true;
}

TArray<AActor*> AWeaponFunctionality::GetAllEnemies() const
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), Enemies);
	return Enemies;
}

int AWeaponFunctionality::GetEnemiesWithinWeaponRange(TArray<AEnemy*>& EnemiesInRange)
{
	GetEnemiesWithinRange(EnemiesInRange, WeaponStats->Range);
	return EnemiesInRange.Num();
}

int AWeaponFunctionality::GetEnemiesWithinRange(TArray<AEnemy*>& EnemiesInRange, float Radius)
{
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), AllEnemies);

	const float RadiusSquared = Radius * Radius;
	for (AActor* Enemy : AllEnemies)
	{
		if (FVector::DistSquared(Enemy->GetActorLocation(), OwningCharacter->GetActorLocation()) <= RadiusSquared)
			EnemiesInRange.Add(Cast<AEnemy>(Enemy));
	}

	return EnemiesInRange.Num();
}

int AWeaponFunctionality::GetEnemiesWithinRangeSorted(TArray<AEnemy*>& OutEnemiesInRange, float Radius)
{
	GetEnemiesWithinRange(OutEnemiesInRange, Radius);
	OutEnemiesInRange.Sort([this] (const AEnemy& A, const AEnemy& B) {
		return FVector::DistSquared(A.GetActorLocation(), OwningCharacter->GetActorLocation()) <
			FVector::DistSquared(B.GetActorLocation(), OwningCharacter->GetActorLocation());
	});
	return OutEnemiesInRange.Num();
}

AEnemy* AWeaponFunctionality::GetClosestEnemyWithinWeaponRange()
{
	float ClosestDistance = FLT_MAX;
	AEnemy* ClosestEnemy = nullptr;

	TArray<AEnemy*> Enemies;
	GetEnemiesWithinRange(Enemies, WeaponStats->Range);
	for (AEnemy* Enemy : Enemies)
	{
		const float Distance = FVector::DistSquared(Enemy->GetActorLocation(), OwningCharacter->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestEnemy = Enemy;
		}
	}

	return ClosestEnemy;
}

AProjectile* AWeaponFunctionality::SpawnProjectile(const FVector& Position, const FVector& Direction,
	UProjectileData* ProjectileType) const
{
	AProjectile* NewProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileType->ProjectileSubclass, FTransform(Position));
	NewProjectile->Initialise(OwningCharacter, WeaponStats, ProjectileType);
	
	// Add spread to direction vector
	FVector AdjustedDir = Direction;
	const FRotator Spread(FMath::RandRange(-WeaponStats->Spread, WeaponStats->Spread),
	                      FMath::RandRange(-WeaponStats->Spread, WeaponStats->Spread), 0);
	AdjustedDir = AdjustedDir.RotateAngleAxis(Spread.Pitch, FVector::RightVector);
	AdjustedDir = AdjustedDir.RotateAngleAxis(Spread.Yaw, FVector::UpVector);
	
	NewProjectile->FireInDirection(AdjustedDir);
	return NewProjectile;
}

void AWeaponFunctionality::PostNetInit()
{
	Super::PostNetInit();

	// On the client, duplicate the stats object from the weapon data.
	// The weapon stats object is not replicated directly, but any changes to it via upgrades will be replicated via RPCs.
	WeaponStats = DuplicateObject<UWeaponStats>(WeaponData->BaseWeaponStats, this);

	// And if we're owned by the local controller, update the UI. This feels like a hack, but it works.
	const auto LocalController = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld());
	if (IsOwnedBy(LocalController))
		LocalController->GetPlayerState<APiratePlayerState>()->Client_OnReceivedWeapon(this);
}

bool AWeaponFunctionality::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	// if (WeaponStats != nullptr)
	// {
	// 	WroteSomething |= Channel->ReplicateSubobject(WeaponStats, *Bunch, *RepFlags);
	// }

	return WroteSomething;
}

void AWeaponFunctionality::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AWeaponFunctionality, Ammo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AWeaponFunctionality, ReloadTime, COND_OwnerOnly);
	// DOREPLIFETIME(AWeaponFunctionality, WeaponStats);
	DOREPLIFETIME_CONDITION(AWeaponFunctionality, WeaponData, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AWeaponFunctionality, OwningCharacter, COND_InitialOnly);
}
