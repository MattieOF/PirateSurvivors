// copyright lololol

#include "Weapon/Projectile.h"

#include "PirateLog.h"
#include "PirateSurvivors.h"
#include "Core/PirateSurvivorsCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/ProjectileData.h"
#include "Weapon/WeaponStats.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	// AActor::SetReplicateMovement(true);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->bAutoActivate = false;
	// No gravity by default. This could be changed with a projectile subclass if needed, but we really don't want gravity on most projectiles.
	// Most of the time, the effect you'd get from gravity is achieved with a lifetime.
	ProjectileMovementComponent->ProjectileGravityScale = 0;
	// No friction either. This is a projectile, it's supposed to keep going until it hits something (or its lifetime expires).
	// Again, this could be changed with a projectile subclass if needed.
	ProjectileMovementComponent->Friction = 0;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnProjectileHit);

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
			Destroy();
		}
	}
}

void AProjectile::OnProjectileDeath_Implementation()
{ }

void AProjectile::Initialise(APirateSurvivorsCharacter* NewOwner, UWeaponStats* WeaponStats, UProjectileData* NewData)
{
	if (!WeaponStats)
	{
		PIRATE_LOG_ERROR("%ls called Initialise() with null weapon stats!", *GetName());
		return;
	}
	
	if (!NewData)
	{
		PIRATE_LOG_WARN("%ls called Initialise() with null data, was this intentional?", *GetName());
	}

	OwningCharacter = NewOwner;
	Data = NewData;
	ProjectileSpeed  = WeaponStats->ProjectileSpeed * (Data ? Data->GetRandomSpeedMultiplier() : 1);
	ProjectileDamage = WeaponStats->Damage * (Data ? Data->GetRandomDamageMultiplier() : 1);
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	TimeUntilDeath = Data ? Data->LifeTime : -1;
	if (Data)
		SetupFromData();
	
	if (Data && Data->ProjectileSubclass != GetClass())
	{
		PIRATE_LOG_ERROR("ProjectileData's subclass is not the same as the class of the projectile. This could cause issues.");
	}
}

void AProjectile::FireInDirection(const FVector& NewDirection)
{
	ProjectileDirection = NewDirection;
	ProjectileMovementComponent->Velocity = NewDirection * ProjectileMovementComponent->InitialSpeed;
	ProjectileMovementComponent->Activate();
}

void AProjectile::DamageHealthComponent(UHealthComponent* HealthComponent)
{
	if (!HealthComponent->GetOwner()->HasAuthority())
		return;
	
	FDamageInstance DamageEvent;
	DamageEvent.Damage = ProjectileDamage;
	DamageEvent.Instigator = OwningCharacter;
	if (OwningCharacter)
		DamageEvent.Description = FText::Format(FText::FromString("{0}'s {1}"), OwningCharacter->CharacterName, Data->Name);
	else
		DamageEvent.Description = FText::Format(FText::FromString("A {1}"), Data->Name);
	HealthComponent->TakeDamage(DamageEvent);
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	if (APirateSurvivorsCharacter* Character = Cast<APirateSurvivorsCharacter>(OtherActor))
	{
		ProjectileHitCharacter(Character);
	}
	else
	{
		ProjectileHitNonCharacter(OtherActor);
	}

	if (bDestroyOnHit)
	{
		OnProjectileDeath();
		Destroy();
	}
}

void AProjectile::ProjectileHitNonCharacter_Implementation(AActor* Other)
{
	if (UHealthComponent* Health = Other->GetComponentByClass<UHealthComponent>())
		DamageHealthComponent(Health);
}

void AProjectile::ProjectileHitCharacter_Implementation(APirateSurvivorsCharacter* Other)
{
	DamageHealthComponent(Other->GetHealthComponent());
}

void AProjectile::SetupFromData()
{
	Mesh->SetStaticMesh(Data->Mesh);

	// Setup collision
	if (!Data->bCollideWithWalls)                    // If we don't collide with walls, disable all collisions.
		Mesh->SetCollisionProfileName("IgnoreAll");  // We'll re-enable the channels we want to collide with in the following lines.
	Mesh->SetCollisionResponseToChannel(ECC_Player, Data->bCollideWithPlayers ? ECR_Block : ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Enemy, Data->bCollideWithEnemies ? ECR_Block : ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore); // We don't want projectiles to collide with each other
	Mesh->SetCollisionObjectType(ECC_Projectile);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AProjectile::PostNetInit()
{
	Super::PostNetInit();
	SetupFromData();
	ProjectileMovementComponent->Velocity = ProjectileDirection * ProjectileSpeed;
	ProjectileMovementComponent->Activate();
}

void AProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AProjectile, Data, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AProjectile, ProjectileSpeed, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AProjectile, ProjectileDirection, COND_InitialOnly);
}

void AProjectile::Multicast_OnProjectileDeath_Implementation()
{
	OnProjectileDeath();
}
