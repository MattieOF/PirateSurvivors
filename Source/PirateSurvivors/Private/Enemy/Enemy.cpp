﻿// copyright lololol

#include "Enemy/Enemy.h"

#include "PirateLog.h"
#include "PirateSurvivors.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyData.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetUpdateFrequency = 5;
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_Enemy);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetData(EnemyData);
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
		PIRATE_LOG_ERROR(FString::Printf(TEXT("In enemy %s, SetData called with null enemydata"), *GetName()));
	}
	
	GetMesh()->SetSkeletalMesh(EnemyData ? EnemyData->Mesh : nullptr);
	if (HasAuthority() && EnemyData && HealthComponent)
		HealthComponent->SetHP(EnemyData->BaseHealth, true);
}

void AEnemy::Multicast_SetData_Implementation(UEnemyData* NewEnemyData)
{
	SetData(NewEnemyData);
}