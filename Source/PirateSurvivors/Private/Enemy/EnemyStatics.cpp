// copyright lololol

#include "Enemy/EnemyStatics.h"

#include "EngineUtils.h"
#include "Core/PiratePlayerCharacter.h"

APiratePlayerCharacter* UEnemyStatics::GetClosestPlayer(UObject* WorldContextObject, const FVector& Location, float& OutDistance, bool bIncludeDead)
{
	APiratePlayerCharacter* ClosestPlayer = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	for (TActorIterator<APiratePlayerCharacter> It(GEngine->GetWorldFromContextObjectChecked(WorldContextObject)); It; ++It)
	{
		APiratePlayerCharacter* Player = *It;
		
		if (!Player)
			continue;

		if (!bIncludeDead && Player->GetHealthComponent() && Player->GetHealthComponent()->IsDead())
			continue;

		const float Distance = FVector::DistSquared(Location, Player->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestPlayer = Player;
			ClosestDistance = Distance;
		}
	}

	if (ClosestPlayer)
		OutDistance = FMath::Sqrt(ClosestDistance);
	return ClosestPlayer;
}
