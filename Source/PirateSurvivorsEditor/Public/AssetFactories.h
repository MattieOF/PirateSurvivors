﻿// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "AssetFactories.generated.h"

#define DECLARE_ASSET_FACTORY(AssetName) UCLASS()\
class U##AssetName##Factory : public UFactory\
{\
	GENERATED_BODY()\
	\
public:\
	U##AssetName##Factory();\
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;\
}

namespace FactoryStatics
{
	static FString GetAssetNameWithoutPrefix(FString Name);
	static FString HumanizeString(FString Input);
};

// These are all generated via the DECLARE_ASSET_FACTORY macro, but expanded here via ReSharper as the macro doesn't compile.
// Example: DECLARE_ASSET_FACTORY(WeaponData);
UCLASS() class UWeaponDataFactory : public UFactory { GENERATED_BODY() public: UWeaponDataFactory(); virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override; };
UCLASS() class UEnemyDataFactory : public UFactory { GENERATED_BODY() public: UEnemyDataFactory(); virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override; };
UCLASS() class UPlayerClassDataFactory : public UFactory { GENERATED_BODY() public: UPlayerClassDataFactory(); virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override; };
UCLASS() class UProjectileDataFactory : public UFactory { GENERATED_BODY() public: UProjectileDataFactory(); virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override; };
UCLASS() class UPlayerStatsFactory : public UFactory { GENERATED_BODY() public: UPlayerStatsFactory(); virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override; };
UCLASS() class UWeaponUpgradeFactory : public UFactory { GENERATED_BODY() public: UWeaponUpgradeFactory(); virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override; };
UCLASS() class UPlayerUpgradeFactory : public UFactory { GENERATED_BODY() public: UPlayerUpgradeFactory(); virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override; };
UCLASS() class UEncounterDataFactory : public UFactory { GENERATED_BODY() public: UEncounterDataFactory(); virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override; };
UCLASS() class UWeaponStatsFactory : public UFactory { GENERATED_BODY() public: UWeaponStatsFactory(); virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override; };
