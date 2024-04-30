// copyright lololol

#include "Enemy/EnemyAIController.h"

#include "PirateLog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/PiratePlayerCharacter.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyData.h"
#include "Enemy/EnemyStatics.h"
#include "Enemy/EnemyStats.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::SetTarget(APirateSurvivorsCharacter* NewTarget)
{
	Target = NewTarget;
	bIsTargetPlayer = Cast<APiratePlayerCharacter>(NewTarget) != nullptr;
	GetBlackboardComponent()->SetValueAsObject(TargetPropertyName, NewTarget);
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

	// Setup some basic stats
	if (Data->Stats)
	{
		PossessedEnemy->GetCharacterMovement()->MaxWalkSpeed = Data->Stats->WalkSpeed;
	}
	else
		PIRATE_LOGC_ERROR(GetWorld(), "Null enemy stats in EnemyData %s", *Data->GetName());

	// For now, just set the target to the nearest player.
	if (APiratePlayerCharacter* ClosestPlayer = UEnemyStatics::GetClosestPlayerNoDist(this, PossessedEnemy->GetActorLocation()))
		SetTarget(ClosestPlayer);
	else
		PIRATE_LOGC_WARN(GetWorld(), "No players found to target!");

	// Initialise other blackboard data
	for (const TPair<FName, float>& Pair : Data->DefaultAIValues)
		BB->SetValueAsFloat(Pair.Key, Pair.Value);
}
