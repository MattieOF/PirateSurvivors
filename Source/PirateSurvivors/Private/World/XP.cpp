// copyright lololol

#include "World/XP.h"

AXP::AXP()
{
	PrimaryActorTick.bCanEverTick = false;

	SetActorEnableCollision(true);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = Mesh;
}

void AXP::Hide()
{
	Mesh->SetVisibility(false);
}
