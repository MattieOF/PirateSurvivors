// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "WeaponFunctionality.h"
#include "BasicSummonWeapon.generated.h"

class ASummon;

UCLASS()
class PIRATESURVIVORS_API ABasicSummonWeapon : public AWeaponFunctionality
{
	GENERATED_BODY()

public:
	ABasicSummonWeapon();

	virtual void OnFire_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summon Weapon")
	TSubclassOf<ASummon> SummonClass;
};
