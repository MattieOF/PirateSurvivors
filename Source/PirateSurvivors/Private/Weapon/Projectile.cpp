// copyright lololol

#include "Weapon/Projectile.h"

#include "PirateLog.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapon/ProjectileData.h"
#include "Weapon/WeaponStats.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->bAutoActivate = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Mesh;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.SetTickFunctionEnable(HasAuthority());
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TimeUntilDeath > 0)
	{
		TimeUntilDeath -= DeltaTime;
		if (TimeUntilDeath <= 0)
		{
			Multicast_OnProjectileDeath();
		}
	}
}

void AProjectile::OnProjectileDeath_Implementation()
{
	Destroy();
}

void AProjectile::Initialise(UWeaponStats* WeaponStats, UProjectileData* NewData)
{
	if (!WeaponStats)
	{
		PIRATE_LOG_WARN(FString::Printf(TEXT("%ls called Initialise() with null weapon stats!"), *GetName()));
		return;
	}
	
	if (!Data)
	{
		PIRATE_LOG_WARN(FString::Printf(TEXT("%ls called Initialise() with null data, was this intentional?"), *GetName()));
	}
	
	Data = NewData;
	ProjectileMovementComponent->InitialSpeed = WeaponStats->ProjectileSpeed * (Data ? Data->GetRandomSpeedMultiplier() : 1);
	TimeUntilDeath = Data ? Data->LifeTime : -1;
	if (Data)
	{
		Mesh->SetStaticMesh(Data->Mesh);
		ProjectileMovementComponent->Activate();
	}
	
	if (Data && Data->ProjectileSubclass != StaticClass())
	{
		PIRATE_LOG_ERROR("ProjectileData's subclass is not the same as the class of the projectile. This could cause issues.");
	}
}

void AProjectile::SetDirection(const FVector& NewDirection)
{
	ProjectileMovementComponent->Velocity = NewDirection;
}

void AProjectile::Multicast_OnProjectileDeath_Implementation()
{
	OnProjectileDeath();
}
