// copyright lololol

#include "ActorFactoryEnemy.h"

#include "LevelEditorViewport.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyData.h"
#include "Subsystems/PlacementSubsystem.h"

UActorFactoryEnemy::UActorFactoryEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("ActorFactory", "EnemyDisplayName", "Enemy");
	NewActorClass = AEnemy::StaticClass();
	bUseSurfaceOrientation = true;
}

bool UActorFactoryEnemy::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if ( !AssetData.IsValid() || !AssetData.IsInstanceOf( UEnemyData::StaticClass() ) )
	{
		OutErrorMsg = NSLOCTEXT("CanCreateActor", "NoEnemyData", "A valid enemy data object must be specified.");
		return false;
	}

	return true;
}

AActor* UActorFactoryEnemy::SpawnActor(UObject* InAsset, ULevel* InLevel, const FTransform& InTransform,
	const FActorSpawnParameters& InSpawnParams)
{
	ULevel* LocalLevel = ValidateSpawnActorLevel(InLevel, InSpawnParams);

	const UEnemyData* EnemyData = CastChecked<UEnemyData>(InAsset);;
	if ((EnemyData->EnemySubclass != nullptr) && (LocalLevel != nullptr))
	{
		FActorSpawnParameters SpawnParamsCopy(InSpawnParams);
		SpawnParamsCopy.OverrideLevel = LocalLevel;

		const bool bIsCreatingPreviewElements = FLevelEditorViewportClient::IsDroppingPreviewActor();
		bool bIsPlacementSystemCreatingPreviewElements = false;
		if (UPlacementSubsystem* PlacementSubsystem = GEditor->GetEditorSubsystem<UPlacementSubsystem>())
		{
			bIsPlacementSystemCreatingPreviewElements = PlacementSubsystem->IsCreatingPreviewElements();
		}
		SpawnParamsCopy.bTemporaryEditorActor = bIsCreatingPreviewElements || bIsPlacementSystemCreatingPreviewElements;
		SpawnParamsCopy.bHideFromSceneOutliner = bIsPlacementSystemCreatingPreviewElements;

		return LocalLevel->OwningWorld->SpawnActor(EnemyData->EnemySubclass, &InTransform, SpawnParamsCopy);
	}

	return nullptr;
}

void UActorFactoryEnemy::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	UEnemyData* EnemyData = CastChecked<UEnemyData>(Asset);

	// Change properties
	AEnemy* Enemy = CastChecked<AEnemy>( NewActor );
	Enemy->SetData(EnemyData);
}

void UActorFactoryEnemy::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	if (Asset != nullptr && CDO != nullptr)
	{
		UEnemyData* EnemyData = CastChecked<UEnemyData>(Asset);
		AEnemy* Enemy = CastChecked<AEnemy>(CDO);
		Enemy->SetData(EnemyData);
	}
}

UObject* UActorFactoryEnemy::GetAssetFromActorInstance(AActor* ActorInstance)
{
	check(ActorInstance->IsA(NewActorClass));
	const AEnemy* Enemy = CastChecked<AEnemy>(ActorInstance);
	return Enemy->GetData();
}

FQuat UActorFactoryEnemy::AlignObjectToSurfaceNormal(const FVector& InSurfaceNormal, const FQuat& ActorRotation) const
{
	// Meshes align the Z (up) axis with the surface normal
	return FindActorAlignmentRotation(ActorRotation, FVector(0.f, 0.f, 1.f), InSurfaceNormal);
}
