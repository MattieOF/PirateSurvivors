// copyright lololol

#include "Enemy/EnemyAIController.h"

#include "PirateLog.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyData.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PIRATE_LOGC(GetWorld(), "Possessing enemy: %s", *InPawn->GetName());

	PossessedEnemy = Cast<AEnemy>(InPawn);
	if (!PossessedEnemy)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Possessed pawn is not an enemy!");
		return;
	}

	const UEnemyData* Data = PossessedEnemy->GetData();
	if (!Data)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Enemy data is null!");
		return;
	}
	
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (Data->Blackboard)
		UseBlackboard(Data->Blackboard, BB);
	if (Data->BehaviorTree)
		RunBehaviorTree(Data->BehaviorTree);
}
