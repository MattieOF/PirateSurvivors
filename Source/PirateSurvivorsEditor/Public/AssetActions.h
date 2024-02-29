// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "AssetDefinitionDefault.h"
#include "AssetTypeActions_Base.h"

#include "Enemy/EnemyData.h"
#include "Player/PlayerClassData.h"
#include "Player/PlayerStats.h"
#include "Player/Upgrade.h"
#include "Weapon/WeaponData.h"

#include "AssetActions.generated.h"

#define LOCTEXT_NAMESPACE "PirateSurvivorsEditorModule"

#define DECLARE_ASSET_DEFINITION(Type, KeyName, TypeName, Color) UCLASS()\
class UAssetDefinition_##KeyName : public UAssetDefinitionDefault\
{\
	GENERATED_BODY()\
\
public:\
	virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_"#KeyName, ##TypeName##); }\
	virtual FLinearColor GetAssetColor() const override { return Color; };\
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return Type::StaticClass(); }\
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override\
	{\
		static const auto Categories = {\
			EAssetCategoryPaths::Basic / LOCTEXT("PirateAssetSubMenu", "Pirate Survivors")\
		};\
		\
		return Categories;\
	}\
}

// These are all generated via the DECLARE_ASSET_DEFINITION macro, but expanded here via ReSharper as the macro doesn't compile.
UCLASS() class UAssetDefinition_WeaponData : public UAssetDefinitionDefault { GENERATED_BODY() public: virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_""WeaponData","Weapon Data"); } virtual FLinearColor GetAssetColor() const override { return FLinearColor::Blue; }; virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UWeaponData::StaticClass(); } virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override { static const auto Categories = { EAssetCategoryPaths::Basic / LOCTEXT("PirateAssetSubMenu", "Pirate Survivors") }; return Categories; } };
UCLASS() class UAssetDefinition_EnemyData : public UAssetDefinitionDefault { GENERATED_BODY() public: virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_""EnemyData","Enemy Data"); } virtual FLinearColor GetAssetColor() const override { return FLinearColor::Red; }; virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UEnemyData::StaticClass(); } virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override { static const auto Categories = { EAssetCategoryPaths::Basic / LOCTEXT("PirateAssetSubMenu", "Pirate Survivors") }; return Categories; } };
UCLASS() class UAssetDefinition_PlayerClassData : public UAssetDefinitionDefault { GENERATED_BODY() public: virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_""PlayerClassData","Player Class Data"); } virtual FLinearColor GetAssetColor() const override { return FLinearColor::Green; }; virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UPlayerClassData::StaticClass(); } virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override { static const auto Categories = { EAssetCategoryPaths::Basic / LOCTEXT("PirateAssetSubMenu", "Pirate Survivors") }; return Categories; } };
UCLASS() class UAssetDefinition_PlayerStats : public UAssetDefinitionDefault { GENERATED_BODY() public: virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_""PlayerStats","Player Stats"); } virtual FLinearColor GetAssetColor() const override { return FLinearColor::Green; }; virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UPlayerStats::StaticClass(); } virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override { static const auto Categories = { EAssetCategoryPaths::Basic / LOCTEXT("PirateAssetSubMenu", "Pirate Survivors") }; return Categories; } };
UCLASS() class UAssetDefinition_WeaponUpgrade : public UAssetDefinitionDefault { GENERATED_BODY() public: virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_""WeaponUpgrade","Weapon Upgrade"); } virtual FLinearColor GetAssetColor() const override { return FLinearColor::Green; }; virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UWeaponUpgrade::StaticClass(); } virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override { static const auto Categories = { EAssetCategoryPaths::Basic / LOCTEXT("PirateAssetSubMenu", "Pirate Survivors") / LOCTEXT("PirateUpgradeAssetSubMenu", "Upgrades") }; return Categories; } };
UCLASS() class UAssetDefinition_PlayerUpgrade : public UAssetDefinitionDefault { GENERATED_BODY() public: virtual FText GetAssetDisplayName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_""PlayerUpgrade","Player Upgrade"); } virtual FLinearColor GetAssetColor() const override { return FLinearColor::Green; }; virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UPlayerUpgrade::StaticClass(); } virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override { static const auto Categories = { EAssetCategoryPaths::Basic / LOCTEXT("PirateAssetSubMenu", "Pirate Survivors") / LOCTEXT("PirateUpgradeAssetSubMenu", "Upgrades") }; return Categories; } };

#undef LOCTEXT_NAMESPACE
