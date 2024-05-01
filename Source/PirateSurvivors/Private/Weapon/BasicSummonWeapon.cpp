// copyright lololol

#include "Weapon/BasicSummonWeapon.h"

#include "NavigationSystem.h"
#include "PirateLog.h"
#include "Core/PirateSurvivorsCharacter.h"
#include "Weapon/Summon.h"
#include "Weapon/WeaponStats.h"

ABasicSummonWeapon::ABasicSummonWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bRequestReposition = false;
	SummonClass = ASummon::StaticClass();
}

void ABasicSummonWeapon::OnFire_Implementation()
{
	Super::OnFire_Implementation();

	// Find a random point on navmesh in radius
	const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this->GetWorld());
	if (!NavSystem)
	{
		PIRATE_LOGC_ERROR_NOLOC(GetWorld(), "In ABasicSummonWeapon::OnFire: NavSystem is null");
		return;
	}
	FNavLocation OutLoc;
	NavSystem->GetRandomPointInNavigableRadius(OwningCharacter->GetActorLocation(), WeaponStats->Range, OutLoc);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwningCharacter;
	ASummon* Summon = Cast<ASummon>(GetWorld()->SpawnActor(SummonClass, &OutLoc.Location, &FRotator::ZeroRotator, SpawnParams));
	check(Summon);
	FVector Extent;
	Summon->GetActorBounds(true, OutLoc.Location, Extent);
	Summon->AddActorWorldOffset(FVector(0, 0, Extent.Z));
	Summon->OwningCharacter = OwningCharacter;
	Summon->Stats = WeaponStats;
	
	UseAmmo(1);
}
