// copyright lololol

#include "World/XP.h"

#include "PirateSurvivors.h"

AXP::AXP()
{
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetActorEnableCollision(true);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// We don't want to collide with anything other than 
	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	RootComponent = Mesh;
}

void AXP::Hide()
{
	Mesh->SetVisibility(false);
}

void AXP::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PickupDelay > 0)
	{
		PickupDelay -= DeltaSeconds;
		if (PickupDelay <= 0)
		{
			bCanBePickedUp = true;
			PrimaryActorTick.SetTickFunctionEnable(false);
		}
	}
}

void AXP::SetPhysics(FVector Velocity)
{
	bCanBePickedUp = false;
	PickupDelay = 1;
	PrimaryActorTick.SetTickFunctionEnable(false);
	
	Mesh->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	Mesh->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Enemy, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
	
	bPhysicsEnabled = true;
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	Mesh->SetPhysicsLinearVelocity(Velocity);
}
