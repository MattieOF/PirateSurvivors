// copyright lololol

#include "Enemy/Enemy.h"

#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyData.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetUpdateFrequency = 5;
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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

	GetMesh()->SetSkeletalMesh(EnemyData->Mesh);
	if (HasAuthority())
		HealthComponent->SetHP(EnemyData->BaseHealth, true);
}

void AEnemy::Multicast_SetData_Implementation(UEnemyData* NewEnemyData)
{
	SetData(NewEnemyData);
}
