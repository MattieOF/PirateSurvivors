// copyright lololol

#include "Player/Upgrade.h"

#include "PirateLog.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerController.h"
#include "Core/PiratePlayerState.h"
#include "Player/PlayerStats.h"
#include "Weapon/WeaponData.h"
#include "Weapon/WeaponFunctionality.h"
#include "Weapon/WeaponStats.h"

UWeaponUpgrade::UWeaponUpgrade()
{
	WeaponStatsClass = UWeaponStats::StaticClass();
}

TArray<FName> UWeaponUpgrade::GetPropertyNames()
{
	TArray<FName> PropertyNames;
	for (TFieldIterator<FProperty> Prop(WeaponStatsClass ? WeaponStatsClass : UWeaponStats::StaticClass()); Prop; ++Prop) {
		const FProperty* Property = *Prop;
		// Continue if the property is not a float
		if (Property->GetClass() != FFloatProperty::StaticClass()) continue;
		PropertyNames.Add(Property->GetFName());
	}
	return PropertyNames;
}

bool UWeaponUpgrade::VerifyCompatability() const
{
	if (!WeaponStatsClass)
	{
		PIRATE_LOG_ERROR_NOLOC("Weapon upgrade %ls has no weapon stats class! This must be set for the upgrade to work.", *GetName());
		return false;
	}
	
	if (ValidWeapons.Num() == 0)
	{
		PIRATE_LOG_ERROR_NOLOC("Weapon upgrade %ls has no valid weapons. This probably isn't intentional!", *GetName());
		return false;
	}

	for (const FWeaponStatUpgrade& Upgrade : StatUpgrades)
	{
		if (WeaponStatsClass.Get()->FindPropertyByName(Upgrade.PropertyName) == nullptr)
		{
			PIRATE_LOG_ERROR_NOLOC("Weapon upgrade %ls has an invalid property name: %ls", *GetName(), *Upgrade.PropertyName.ToString());
			return false;
		}
	}

	for (const FWeaponFunctionalityUpgrade& Upgrade : FunctionalityUpgrades)
	{
		if (Upgrade.Type == EWeaponFunctionalityUpgradeType::ChangeClass && !Upgrade.ClassValue)
		{
			PIRATE_LOG_ERROR_NOLOC("Weapon upgrade %ls has a functionality upgrade that changes the class of the weapon, but the class is null!", *GetName());
			return false;
		}
	}

	for (const UWeaponData* Weapon : ValidWeapons)
	{
		if (!Weapon)
		{
			PIRATE_LOG_ERROR_NOLOC("Weapon upgrade %ls has a null valid weapon!", *GetName());
			return false;
		}

		if (!Weapon->WeaponStatsSubclass)
		{
			PIRATE_LOG_ERROR_NOLOC("Weapon upgrade %ls has weapon %ls set as valid, but the weapon has an unset weapon stats class!",
					*GetName(), *Weapon->GetName());
			return false;
		}

		if (!Weapon->WeaponStatsSubclass.Get()->IsChildOf(WeaponStatsClass))
		{
			PIRATE_LOG_ERROR_NOLOC("Weapon upgrade %ls has weapon %ls set as valid, but it has an incompatible weapon stats class (%s)!"
						" They must either be the same, or the weapons stat class must be a subclass of this upgrades stat class.",
					*GetName(), *Weapon->GetName(), *Weapon->WeaponStatsSubclass.Get()->GetName());
			return false;
		}

		if (!Weapon->WeaponFunctionalitySubclass)
		{
			PIRATE_LOG_ERROR_NOLOC("Weapon upgrade %ls has weapon %ls set as valid, but the weapon has an unset weapon functionality class!",
					*GetName(), *Weapon->GetName());
			return false;
		}

		for (const FWeaponFunctionalityUpgrade& Upgrade : FunctionalityUpgrades)
		{
			if (Upgrade.Type != EWeaponFunctionalityUpgradeType::ChangeClass)
			{
				if (!Weapon->WeaponFunctionalitySubclass.Get()->FindPropertyByName(Upgrade.PropertyName))
				{
					PIRATE_LOG_ERROR_NOLOC("Weapon functionality upgrade attempts to modify property %ls, but valid weapon %ls's functionality class does not have this property!",
							*Upgrade.PropertyName.ToString(), *Weapon->GetName());
					return false;
				}
			}
		}
	}
	
	return true;
}

AWeaponFunctionality* UWeaponUpgrade::ApplyUpgrade(APiratePlayerState* Player, AWeaponFunctionality* Weapon)
{
	AWeaponFunctionality* ReturnValue = Weapon;
	
	for (const FWeaponFunctionalityUpgrade& Upgrade : FunctionalityUpgrades)
	{
		switch (Upgrade.Type)
		{
		case EWeaponFunctionalityUpgradeType::SetBool:
		case EWeaponFunctionalityUpgradeType::SetFloat:
		case EWeaponFunctionalityUpgradeType::AddFloat:
			{
				FProperty* Property = Weapon->GetClass()->FindPropertyByName(Upgrade.PropertyName);
				switch (Upgrade.Type)
				{
				case EWeaponFunctionalityUpgradeType::SetFloat:
				case EWeaponFunctionalityUpgradeType::AddFloat:
					{
						const FFloatProperty* FloatProp = CastField<FFloatProperty>(Property);
						if (!FloatProp)
						{
							PIRATE_LOGC_ERROR(GetWorld(), "Weapon functionality upgrade on %ls tries to set non-float property as float: %ls", *Name.ToString(), *Upgrade.PropertyName.ToString());
							break;
						}
				
						float NewValue = 0;
						if (Upgrade.Type == EWeaponFunctionalityUpgradeType::SetFloat)
							NewValue = Upgrade.FloatValue;
						else
						{
							float Current = 0;
							FloatProp->GetValue_InContainer(Weapon, &Current);
							NewValue = Current + Upgrade.FloatValue;
						}

						if (FloatProp->HasSetter())
							FloatProp->CallSetter(Weapon, &NewValue);
						else
							FloatProp->SetPropertyValue_InContainer(Weapon, NewValue);
					}
					break;
				case EWeaponFunctionalityUpgradeType::SetBool:
					{
						if (const FBoolProperty* BoolProp = CastField<FBoolProperty>(Property))
						{
							if (BoolProp->HasSetter())
								BoolProp->CallSetter(Weapon, &Upgrade.bBoolValue);
							else
								BoolProp->SetPropertyValue_InContainer(Weapon, Upgrade.bBoolValue);
						}
						else
							PIRATE_LOGC_ERROR(GetWorld(), "Weapon functionality upgrade on %ls tries to set non-bool property as bool: %ls", *Name.ToString(), *Upgrade.PropertyName.ToString());
					}
					break;
				default:
					PIRATE_LOGC_ERROR(GetWorld(), "Unhandled property type in UWeaponUpgrade::ApplyUpgrade/FunctionalityUpgrades (occured in %ls, type %d)", *Name.ToString(), static_cast<int>(Upgrade.Type));
					break;
				}
			}
			break;
		case EWeaponFunctionalityUpgradeType::ChangeClass:
			{
				// First, spawn the new weapon functionality actor
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = Player->GetPirateController();
				AWeaponFunctionality* NewWeapon = GetWorld()->SpawnActor<AWeaponFunctionality>(Upgrade.ClassValue, SpawnParams);
				NewWeapon->InitialiseLight(Player->GetPiratePawn(), Weapon->GetWeaponData());
				NewWeapon->OnDataUpdated.Broadcast();

				// Now we've spawned the new functionality actor, we have to transfer the old one's state to the new one
				// This is done by copying the old one's properties to the new one
				for (TFieldIterator<FProperty> Prop(Weapon->GetClass()); Prop; ++Prop)
				{
					const FProperty* OriginalProperty = *Prop;
					const FProperty* NewWeaponProperty = NewWeapon->GetClass()->FindPropertyByName(OriginalProperty->GetFName());
					if (NewWeaponProperty && NewWeaponProperty->GetSize() == OriginalProperty->GetSize())
					{
						void* ValueAddress = nullptr;
						OriginalProperty->GetValue_InContainer(Weapon, ValueAddress);
						NewWeaponProperty->SetValue_InContainer(NewWeapon, ValueAddress);
					}
				}

				// Now we've copied the properties, we can destroy the old weapon
				// This will cause any code that still has a reference to the old weapon to crash, but that's their fault
				Weapon->Destroy();

				// Finally, set the new weapon as the return value
				// The caller will have to set the weapon in the player state
				ReturnValue = NewWeapon;
			}
			
			break;
		case EWeaponFunctionalityUpgradeType::CallFunction:
			{
				// First, let's find the function we're supposed to call
				// If we found the function, call it
				if (UFunction* Function = Weapon->GetClass()->FindFunctionByName(Upgrade.FunctionToCall))
				{
#if !UE_BUILD_SHIPPING
					// Warn if the function has parameters, as we're not passing any
					if (Function->NumParms != 0)
						PIRATE_LOGC_WARN(GetWorld(), "Weapon functionality upgrade on %ls has a function with parameters, which will be ignored", *Name.ToString());
#endif
					Weapon->ProcessEvent(Function, nullptr);
				}
				else
				{
					PIRATE_LOGC_ERROR(GetWorld(), "Weapon functionality upgrade on %ls has an invalid function name: %ls", *Name.ToString(), *Upgrade.FunctionToCall.ToString());
				}
			}
			
			break;
		default:
			PIRATE_LOGC_ERROR(GetWorld(), "Weapon functionality upgrade on %ls has an invalid type: %d", *Name.ToString(), static_cast<int>(Upgrade.Type));
		}
	}
	
	for (const FWeaponStatUpgrade& Upgrade : StatUpgrades)
	{
		UWeaponStats* Stats = Weapon->GetWeaponStats();
		
		const FFloatProperty* Prop = FindFProperty<FFloatProperty>(Stats->GetClass(), Upgrade.PropertyName);
		if (!Prop)
		{
			PIRATE_LOGC_ERROR(GetWorld(), "Weapon upgrade on %ls has an invalid property name: %ls", *Name.ToString(), *Upgrade.PropertyName.ToString());
			continue;
		}

		float NewValue = Prop->GetPropertyValue_InContainer(Stats);
		if (Upgrade.UpgradeType == EUpgradeType::Add)
			NewValue += Upgrade.Delta;
		else if (Upgrade.UpgradeType == EUpgradeType::Multiply)
			NewValue *= Upgrade.Delta;
		else
		{
			PIRATE_LOGC_ERROR(GetWorld(), "Weapon upgrade on %ls has an invalid upgrade type: %d", *Name.ToString(), static_cast<int>(Upgrade.UpgradeType));
			continue;
		}

		Prop->SetPropertyValue_InContainer(Stats, NewValue);
	}

	return ReturnValue;
}

bool UWeaponUpgrade::IsValidForWeapon(AWeaponFunctionality* Weapon) const
{
	if (!Weapon)
		return false;
	return (ValidWeapons.IsEmpty() && ValidWeaponTags.IsEmpty()) // No conditions
			|| ValidWeapons.Contains(Weapon->GetWeaponData()) // Is a valid weapon type
			|| Weapon->GetWeaponData()->Tags.ContainsByPredicate([this](const FName& Tag) { return ValidWeaponTags.Contains(Tag); }); // Has a valid tag
}

bool UWeaponUpgrade::IsValidForPlayer(APiratePlayerState* Player) const
{
	for (UUpgrade* Prerequisite : Prerequisites)
	{
		if (!Player->AppliedUpgrades.Contains(Prerequisite))
			return false;
	}

	const auto Weapons = Player->GetWeapons();
	for (AWeaponFunctionality* Weapon : *Weapons)
	{
		if (IsValidForWeapon(Weapon))
			return true;
	}
	
	return false;
}

// TODO: Perhaps this function should be moved to the player state?
void UPlayerUpgrade::ApplyUpgrade(APiratePlayerState* Player) const
{
	for (const FPlayerStatUpgrade& Upgrade : StatUpgrades)
	{
		const FFloatProperty* Prop = FindFProperty<FFloatProperty>(Player->PlayerStats->GetClass(), Upgrade.PropertyName);
		if (!Prop)
		{
			PIRATE_LOGC_ERROR(GetWorld(), "Player upgrade on %ls has an invalid property name: %ls", *Name.ToString(), *Upgrade.PropertyName.ToString());
			continue;
		}

		float NewValue = Prop->GetPropertyValue_InContainer(Player->PlayerStats);
		if (Upgrade.UpgradeType == EUpgradeType::Add)
			NewValue += Upgrade.Delta;
		else if (Upgrade.UpgradeType == EUpgradeType::Multiply)
			NewValue *= Upgrade.Delta;
		else
		{
			PIRATE_LOGC_ERROR(GetWorld(), "Player upgrade on %ls has an invalid upgrade type: %d", *Name.ToString(), static_cast<int>(Upgrade.UpgradeType));
			continue;
		}

		if (Prop->HasSetter())
			Prop->CallSetter(Player->PlayerStats, &NewValue);
		else
			Prop->SetPropertyValue_InContainer(Player->PlayerStats, NewValue);
	}
}

bool UPlayerUpgrade::IsValidForPlayer(APiratePlayerState* Player) const
{
	for (UUpgrade* Prerequisite : Prerequisites)
	{
		if (!Player->AppliedUpgrades.Contains(Prerequisite))
			return false;
	}

	return true;
}
