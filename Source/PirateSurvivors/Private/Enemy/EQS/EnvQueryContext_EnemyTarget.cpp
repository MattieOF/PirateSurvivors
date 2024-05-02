// copyright lololol

#include "Enemy/EQS/EnvQueryContext_EnemyTarget.h"

#include "Enemy/Enemy.h"
#include "Enemy/EnemyAIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_EnemyTarget::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                  FEnvQueryContextData& ContextData) const
{
	AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	AEnemy* Enemy = Cast<AEnemy>(QueryOwner);
	check(Enemy);
	AEnemyAIController* AI = Cast<AEnemyAIController>(Enemy->GetController());
	check(AI);
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, AI->GetTarget());
}
