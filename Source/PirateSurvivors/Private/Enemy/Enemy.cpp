// copyright lololol

#include "Enemy/Enemy.h"

#include "PirateLog.h"
#include "PirateSurvivors.h"
#include "Components/CapsuleComponent.h"
#include "Core/PirateGameState.h"
#include "Core/PiratePlayerCharacter.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyData.h"
#include "Enemy/EnemyStats.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/DamageNumbers.h"
#include "UI/HealthBar.h"
#include "UI/HealthBars.h"
#include "World/XPManager.h"

UEnemyData::UEnemyData()
{
	EnemySubclass = AEnemy::StaticClass();
	EnemyAISubclass = AEnemyAIController::StaticClass();
}

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetUpdateFrequency = 5;
	AIControllerClass = nullptr; // This'll be set properly in SetData
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemy::OnHealthChanged);
	HealthComponent->OnDeath.AddDynamic(this, &AEnemy::Die);
	
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Enemy);

	GetCharacterMovement()->bUseRVOAvoidance = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (EnemyData)
		SetData(EnemyData);
}

// Can't be const because of the OnHealthChanged.AddDynamic
// ReSharper disable once CppMemberFunctionMayBeConst

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

void AEnemy::SetData(UEnemyData* NewEnemyData, APiratePlayerCharacter* Target)
{
	EnemyData = NewEnemyData;

	if (!EnemyData)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "In enemy %s, SetData called with null enemydata", *GetName());
	}
	
	GetMesh()->SetSkeletalMesh(EnemyData ? EnemyData->Mesh : nullptr);
	if (EnemyData->bIsMiniBoss)
		SetHasHealthBar(true);
	if (HasAuthority() && EnemyData && HealthComponent)
	{
		HealthComponent->SetMaxHP(EnemyData->Stats->BaseHealth, true, true);

		// Setup AI controller
		AIControllerClass = EnemyData->EnemyAISubclass;
		if (Controller)
		{
			Controller->Destroy();
			Controller = nullptr;
		}
		SpawnDefaultController();

		if (Target != nullptr)
		{
			if (AEnemyAIController* AIController = Cast<AEnemyAIController>(Controller))
				AIController->SetTarget(Target);
		}
	}
}

void AEnemy::SetHasHealthBar(bool bHasHealthBar)
{
	const APirateGameState* GS = APirateGameState::GetPirateGameState(GetWorld());

	if (!GS || !GS->GetHealthBars())
	{
		// It's possible for this code to be called (by BeginPlay) before the gamestate is initialised, so we'll try again next tick
		GetWorldTimerManager().SetTimerForNextTick([this, bHasHealthBar] { SetHasHealthBar(bHasHealthBar); });
		return;
	}
	
	if (bHasHealthBar && !HealthBar)
	{
		HealthBar = GS->GetHealthBars()->AddHealthBar(EnemyData->Name, HealthComponent);
	}
	else if (!bHasHealthBar && HealthBar)
	{
		GS->GetHealthBars()->RemoveHealthBar(GetHealthComponent());
		HealthBar = nullptr;
	}
}

void AEnemy::Multicast_SetData_Implementation(UEnemyData* NewEnemyData, APiratePlayerCharacter* Target)
{
	SetData(NewEnemyData, Target);
}
