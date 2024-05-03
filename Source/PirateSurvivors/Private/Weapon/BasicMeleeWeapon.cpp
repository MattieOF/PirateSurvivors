// copyright lololol

#include "Weapon/BasicMeleeWeapon.h"

#include "Components/BoxComponent.h"
#include "Enemy/Enemy.h"
#include "Weapon/WeaponData.h"
#include "Weapon/WeaponStats.h"

ABasicMeleeWeapon::ABasicMeleeWeapon()
{
	bRequestReposition = false;
	
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Collision"));
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetVisibility(false);
	WeaponMesh->SetupAttachment(WeaponCollision);
	
	RootComponent = WeaponCollision;
}

void ABasicMeleeWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!OwningCharacter)
		return;
	
	const FVector TargetLoc = OwningCharacter->GetActorLocation() + (LastDir * ((WeaponStats->Range / 2) + 100));
	SetActorLocation(TargetLoc);
	WeaponMesh->SetWorldLocation(OwningCharacter->GetActorLocation() + (LastDir * 20));

	if (bAnimating)
	{
		WeaponMesh->SetRelativeRotation(FRotator(-90, 0, FMath::Lerp(-40, 40, AnimationProgress)));
		AnimationProgress += DeltaSeconds / AnimationTime;
		if (AnimationProgress >= 1)
		{
			bAnimating = false;
			AnimationProgress = 0;
			WeaponMesh->SetVisibility(false);
		}
	}
}

void ABasicMeleeWeapon::OnFire_Implementation()
{
	const AEnemy* Nearest = GetClosestEnemyWithinWeaponRange();
	if (!Nearest)
		return;
	const FVector Dir = (Nearest->GetActorLocation() - OwningCharacter->GetActorLocation()).GetSafeNormal();
	const FVector TargetLoc = OwningCharacter->GetActorLocation() + (Dir * ((WeaponStats->Range / 2) + 50));
	LastDir = Dir;
	SetActorLocation(TargetLoc);
	FRotator Rot = Dir.Rotation();
	Rot.Roll = 0;
	Rot.Pitch = 0;
	SetActorRotation(Rot);
	
	TArray<AActor*> Hits;
	WeaponCollision->UpdateOverlaps();
	WeaponCollision->GetOverlappingActors(Hits, AEnemy::StaticClass());

	for (AActor* Hit : Hits)
	{
		if (const AEnemy* Enemy = Cast<AEnemy>(Hit)) // Should always be true, since we filtered by AEnemy::StaticClass() above.
		{
			FDamageInstance DamageEvent;
			DamageEvent.Damage = WeaponStats->Damage;
			DamageEvent.Instigator = OwningCharacter;
			if (OwningCharacter)
				DamageEvent.Description = FText::Format(FText::FromString("{0}'s {1}"), OwningCharacter->CharacterName, WeaponData->Name);
			else
				DamageEvent.Description = FText::Format(FText::FromString("A {1}"), WeaponData->Name);

			Enemy->GetHealthComponent()->TakeDamage(DamageEvent);
		}
	}

	Multicast_OnSwing(Dir);
	UseAmmo(1);
}

void ABasicMeleeWeapon::OnInitialise_Implementation()
{
	Super::OnInitialise_Implementation();
	WeaponStats->OnRangeChanged.AddDynamic(this, &ABasicMeleeWeapon::OnRangeChanged);
	InitialiseFromStats();
}

// Can't be const because of the dynamic delegate binding.
// ReSharper disable once CppMemberFunctionMayBeConst

void ABasicMeleeWeapon::OnRangeChanged(float _)
{
	InitialiseFromStats();
}

void ABasicMeleeWeapon::Multicast_OnSwing_Implementation(FVector Dir)
{
	LastDir = Dir;
	bAnimating = true;
	AnimationProgress = 0;
	WeaponMesh->SetVisibility(true);
}

void ABasicMeleeWeapon::InitialiseFromStats() const
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponCollision->SetBoxExtent(FVector(WeaponStats->Range, WeaponStats->Range / 2, 50));
}
