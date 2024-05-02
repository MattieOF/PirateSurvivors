// copyright lololol

#include "World/XP.h"

#include "PirateSurvivors.h"

AXP::AXP()
{
	PrimaryActorTick.bCanEverTick = true;
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

			if (bPhysicsEnabled)
			{
				bPhysicsEnabled = false;
				Mesh->SetSimulatePhysics(false);
				Mesh->SetEnableGravity(false);
			}
		}
	}
}

void AXP::SetPhysics(FVector Velocity)
{
	bCanBePickedUp = false;
	PickupDelay = 1.25f;
	PrimaryActorTick.SetTickFunctionEnable(true);
	
	Mesh->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	Mesh->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Enemy, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	bPhysicsEnabled = true;
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	Mesh->SetPhysicsLinearVelocity(Velocity);
}
