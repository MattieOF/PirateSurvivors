// copyright lololol

#include "Enemy/EQS/EnvQueryContext_AllPlayers.h"

#include "Core/PiratePlayerController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_AllPlayers::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                 FEnvQueryContextData& ContextData) const
{
	TArray<AActor*> Pawns;
	for (TPlayerControllerIterator<APiratePlayerController>::ServerAll It(GetWorld()); It; ++It)
	{
		const APiratePlayerController* PC = *It;
		if (APawn* Pawn = PC->GetPawn())
			Pawns.Add(Cast<AActor>(Pawn));
	}
	ContextData.NumValues = Pawns.Num();
	ContextData.RawData.SetNumUninitialized(Pawns.Num());
	for (int32 i = 0; i < Pawns.Num(); i++)
		UEnvQueryItemType_Actor::SetValue(ContextData.RawData.GetData() + i * sizeof(FWeakObjectPtr), Pawns[i]);
}
