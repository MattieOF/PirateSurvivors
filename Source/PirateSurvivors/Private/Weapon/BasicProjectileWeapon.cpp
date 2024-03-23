// copyright lololol

#include "Weapon/BasicProjectileWeapon.h"

#include "PirateLog.h"
#include "Core/PirateSurvivorsCharacter.h"
#include "Enemy/Enemy.h"

ABasicProjectileWeapon::ABasicProjectileWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABasicProjectileWeapon::OnFire_Implementation()
{
	Super::OnFire_Implementation();

#if WITH_EDITOR
	if (!Projectile)
	{
		PIRATE_LOG_ERROR("No projectile set for weapon %ls!", *GetName());
		return;
	}
#endif
	
	const AEnemy* Target = GetClosestEnemyWithinWeaponRange();
	if (!Target)
		return;
	const FVector Dir = (Target->GetActorLocation() - OwningCharacter->GetActorLocation()).GetSafeNormal();
	SpawnProjectile(OwningCharacter->GetActorLocation() + (Dir * 50), Dir, Projectile);
	UseAmmo(AmmoPerProjectile);
}
