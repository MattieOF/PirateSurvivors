﻿// copyright lololol

#include "Enemy/Enemy.h"

#include "PirateLog.h"
#include "PirateSurvivors.h"
#include "Components/CapsuleComponent.h"
#include "Core/PirateGameState.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyData.h"
#include "UI/DamageNumbers.h"
#include "World/XPManager.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetUpdateFrequency = 5;
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemy::OnHealthChanged);
	HealthComponent->OnDeath.AddDynamic(this, &AEnemy::Die);
	
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Enemy);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetData(EnemyData);
}

void AEnemy::OnHealthChanged(float Change, float NewHP)
{
	const auto GS = APirateGameState::GetPirateGameState(GetWorld());
	if (!GS) return;
	
	FVector Origin, Bounds;
	GetActorBounds(true, Origin, Bounds);
	GS->GetDamageNumbers()->AddDamageNumber(Origin + FVector(0, 0, Bounds.Z + 30), -Change); // Invert the change, a change of -20 is a damage of 20
}

void AEnemy::Die()
{
	// Do blueprint stuff
	OnDeath();
	
	if (!HasAuthority()) return;

	// Spawn XP
	AXPManager* XPManager = APirateGameState::GetPirateGameState(GetWorld())->GetXPManager();
	XPManager->SpawnXP(GetActorLocation(), EnemyData->GetXPDrop());
	
	// Destroy the enemy
	Destroy();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetData(UEnemyData* NewEnemyData)
{
	EnemyData = NewEnemyData;

	if (!EnemyData)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "In enemy %s, SetData called with null enemydata", *GetName());
	}
	
	GetMesh()->SetSkeletalMesh(EnemyData ? EnemyData->Mesh : nullptr);
	if (HasAuthority() && EnemyData && HealthComponent)
		HealthComponent->SetHP(EnemyData->BaseHealth, true);
}

void AEnemy::Multicast_SetData_Implementation(UEnemyData* NewEnemyData)
{
	SetData(NewEnemyData);
}
