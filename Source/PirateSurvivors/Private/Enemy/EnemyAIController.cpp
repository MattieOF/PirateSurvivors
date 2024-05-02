// copyright lololol

#include "Enemy/EnemyAIController.h"

#include "PirateLog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PSStatics.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyData.h"
#include "Enemy/EnemySpawner.h"
#include "Enemy/EnemyStatics.h"
#include "Enemy/EnemyStats.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::SetTarget(APirateSurvivorsCharacter* NewTarget)
{
	if (APiratePlayerCharacter* Player = Cast<APiratePlayerCharacter>(Target))
		Player->EnemiesTargetingMe--;
	
	Target = NewTarget;
	bIsTargetPlayer = Cast<APiratePlayerCharacter>(NewTarget) != nullptr;
	GetBlackboardComponent()->SetValueAsObject(TargetPropertyName, NewTarget);
	
	if (APiratePlayerCharacter* Player = Cast<APiratePlayerCharacter>(Target))
		Player->EnemiesTargetingMe++;
}

void AEnemyAIController::SetIsMeleeAttacking(bool bNewValue)
{
	bIsMeleeAttacking = bNewValue;
}

void AEnemyAIController::PerformMeleeAttack() const
{
	check(HasAuthority());
	check(PossessedEnemy);
	check(Target);

	if (FVector::DistSquared(PossessedEnemy->GetActorLocation(), Target->GetActorLocation()) > FMath::Square(
		PossessedEnemy->GetData()->Stats->MeleeRange + PossessedEnemy->GetData()->MeleeDistCheckBias))
	{
		return;
	}
	
	FHitResult Hit;
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(GetPawn());
	GetWorld()->LineTraceSingleByChannel(Hit, PossessedEnemy->GetActorLocation(), Target->GetActorLocation(), ECC_WorldStatic, Params);

	if (Hit.GetActor() == Target)
	{
		Target->GetHealthComponent()->TakeDamage({
			FText::Format(FText::FromString("{0}'s Melee"), PossessedEnemy->CharacterName),
			PossessedEnemy->GetData()->Stats->MeleeDamage, false, PossessedEnemy
		});
	}
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

	// PIRATE_LOGC(GetWorld(), "Possessing enemy: %s", *InPawn->GetName());

	if (!UPSStatics::InActualGame())
		return;

	if (PossessedEnemy)
	{
		PossessedEnemy->GetHealthComponent()->OnDeath.RemoveDynamic(this, &AEnemyAIController::OnPossessedEnemyDeath);
	}
	
	PossessedEnemy = Cast<AEnemy>(InPawn);
	if (!PossessedEnemy)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Possessed pawn is not an enemy!");
		return;
	}

	PossessedEnemy->GetHealthComponent()->OnDeath.AddDynamic(this, &AEnemyAIController::OnPossessedEnemyDeath);

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
		GetBlackboardComponent()->SetValueAsFloat("MeleeRange", Data->Stats->MeleeRange);
	}
	else
		PIRATE_LOGC_ERROR(GetWorld(), "Null enemy stats in EnemyData %s", *Data->GetName());
	
	// // For now, just set the target to the nearest player.
	// if (APiratePlayerCharacter* ClosestPlayer = UEnemyStatics::GetClosestPlayerNoDist(this, PossessedEnemy->GetActorLocation()))
	// 	SetTarget(ClosestPlayer);
	// else
	// 	PIRATE_LOGC_WARN(GetWorld(), "No players found to target!");

	// Initialise other blackboard data
	for (const TPair<FName, float>& Pair : Data->DefaultAIValues)
		BB->SetValueAsFloat(Pair.Key, Pair.Value);
}

void AEnemyAIController::OnPossessedEnemyDeath()
{
	APirateGameModeBase* GM = Cast<APirateGameModeBase>(GetWorld()->GetAuthGameMode());
	check(GM);
	GM->GetEnemySpawner()->AliveEnemies--;
	if (APiratePlayerCharacter* Player = Cast<APiratePlayerCharacter>(Target))
		Player->EnemiesTargetingMe--;
}
