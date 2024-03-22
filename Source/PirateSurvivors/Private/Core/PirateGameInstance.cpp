// copyright lololol

#include "Core/PirateGameInstance.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Enemy/EnemyData.h"
#include "Weapon/WeaponData.h"

void UPirateGameInstance::Init()
{
	Super::Init();
	
	// Get the asset registry module
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
		FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Since asset registry scanning is asyncronous, we initiate a synchronous scan 
	// of the data directories to ensure they've all been discovered.
	TArray<FString> ContentPaths;
	ContentPaths.Add(TEXT("/Game/Weapons"));
	ContentPaths.Add(TEXT("/Game/Enemies"));
	AssetRegistry.ScanPathsSynchronous(ContentPaths);

	// Find all the weapon data assets and add them to the weapons map
	TArray<FAssetData> AssetData;
	const UClass* WeaponClass = UWeaponData::StaticClass();
	AssetRegistry.GetAssetsByClass(FTopLevelAssetPath(WeaponClass->GetPackage()->GetFName(), WeaponClass->GetFName()), AssetData);
	for (auto& Asset : AssetData)
	{
		UWeaponData* Weapon = Cast<UWeaponData>(Asset.GetAsset());
		Weapons.Add(Weapon->GetName(), Weapon);
	}
	
	// Find all the weapon data assets and add them to the weapons map
	AssetData.Empty();
	const UClass* EnemyClass = UEnemyData::StaticClass();
	AssetRegistry.GetAssetsByClass(FTopLevelAssetPath(EnemyClass->GetPackage()->GetFName(), EnemyClass->GetFName()), AssetData);
	for (auto& Asset : AssetData)
	{
		UEnemyData* Enemy = Cast<UEnemyData>(Asset.GetAsset());
		Enemies.Add(Enemy->GetName(), Enemy);
	}
}
