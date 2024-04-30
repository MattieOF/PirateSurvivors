// copyright lololol

#include "Enemy/EQS/EnvQueryTest_DistFromAllPlayers.h"

#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"

UEnvQueryTest_DistFromAllPlayers::UEnvQueryTest_DistFromAllPlayers(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DistanceTo = UEnvQueryContext_Querier::StaticClass();
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

void UEnvQueryTest_DistFromAllPlayers::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	// don't support context Item here, it doesn't make any sense
	TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(DistanceTo, ContextLocations))
	{
		return;
	}

	// TODO: finish this maybe, but i think i found a better way
}

FText UEnvQueryTest_DistFromAllPlayers::GetDescriptionTitle() const
{
	return FText::FromString(FString::Printf(TEXT("%s: to %s"), 
		*Super::GetDescriptionTitle().ToString(),
		*UEnvQueryTypes::DescribeContext(DistanceTo).ToString()));
}

FText UEnvQueryTest_DistFromAllPlayers::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}
