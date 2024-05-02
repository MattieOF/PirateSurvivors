// copyright lololol

#include "Weapon/Summon.h"

ASummon::ASummon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	AActor::SetReplicateMovement(true);
}

void ASummon::BeginPlay()
{
	Super::BeginPlay();
	OnSpawn();
}

void ASummon::Multicast_Die_Implementation()
{
	Die();
}

void ASummon::Die()
{
	OnDie();
}

void ASummon::OnSpawn_Implementation()
{
}

void ASummon::OnDie_Implementation()
{
}
