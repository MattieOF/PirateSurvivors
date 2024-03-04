// copyright lololol

#include "Enemy/Enemy.h"

#include "Enemy/EnemyData.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
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
		HealthComponent->SetHP(EnemyData->BaseHealth);
}
