// copyright lololol

#include "Weapon/Summon.h"

ASummon::ASummon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ASummon::OnSpawn_Implementation()
{
}

void ASummon::OnDie_Implementation()
{
}
