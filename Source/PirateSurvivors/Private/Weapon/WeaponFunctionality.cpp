// copyright lololol

#include "Weapon/WeaponFunctionality.h"

#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Projectile.h"
#include "Weapon/ProjectileData.h"
#include "Weapon/WeaponData.h"
#include "Weapon/WeaponStats.h"

AWeaponFunctionality::AWeaponFunctionality()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeaponFunctionality::OnFire_Implementation()
{ }

void AWeaponFunctionality::BeginPlay()
{
	Super::BeginPlay();
	if (WeaponData)
		Initialise(nullptr, WeaponData);
}

void AWeaponFunctionality::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AWeaponFunctionality::Initialise(APirateSurvivorsCharacter* NewOwner, UWeaponData* Data)
{
	WeaponData = Data;
	if (NewOwner)
		OwningCharacter = NewOwner;
	WeaponStats = DuplicateObject<UWeaponStats>(Data->BaseWeaponStats, this);
}

TArray<AActor*> AWeaponFunctionality::GetAllEnemies() const
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), Enemies);
	return Enemies;
}

TArray<AEnemy*> AWeaponFunctionality::GetEnemiesWithinWeaponRange() const
{
	return GetEnemiesWithinRange(WeaponStats->Range);
}

TArray<AEnemy*> AWeaponFunctionality::GetEnemiesWithinRange(float Radius) const
{
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), AllEnemies);

	TArray<AEnemy*> EnemiesInRange;
	const float RadiusSquared = Radius * Radius;
	for (AActor* Enemy : AllEnemies)
	{
		if (FVector::DistSquared(Enemy->GetActorLocation(), OwningCharacter->GetActorLocation()) <= RadiusSquared)
			EnemiesInRange.Add(Cast<AEnemy>(Enemy));
	}

	return EnemiesInRange;
}

AEnemy* AWeaponFunctionality::GetClosestEnemyWithinWeaponRange() const
{
	float ClosestDistance = FLT_MAX;
	AEnemy* ClosestEnemy = nullptr;

	TArray<AEnemy*> Enemies = GetEnemiesWithinRange(WeaponStats->Range);
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
	AProjectile* NewProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileType->ProjectileSubclass, FTransform());
	NewProjectile->Initialise(WeaponStats, ProjectileType);
	NewProjectile->SetDirection(Direction);
	NewProjectile->FinishSpawning(FTransform(Position));
	return NewProjectile;
}
