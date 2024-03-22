// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "WeaponFunctionality.h"
#include "BasicProjectileWeapon.generated.h"

UCLASS()
class PIRATESURVIVORS_API ABasicProjectileWeapon : public AWeaponFunctionality
{
	GENERATED_BODY()

public:
	ABasicProjectileWeapon();
	
	virtual void OnFire_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Weapon")
	UProjectileData* Projectile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Weapon")
	float AmmoPerProjectile = 1;
};
