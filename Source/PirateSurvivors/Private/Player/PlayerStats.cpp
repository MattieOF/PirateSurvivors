#include "Player/PlayerStats.h"

DECLARE_PIRATE_STAT_CPP(UPlayerStats, MaxHealth);
DECLARE_PIRATE_STAT_CPP(UPlayerStats, MaxSpeed);
DECLARE_PIRATE_STAT_CPP(UPlayerStats, Armor);
DECLARE_PIRATE_STAT_CPP(UPlayerStats, Luck);
DECLARE_PIRATE_STAT_CPP(UPlayerStats, UpgradeChoices);
DECLARE_PIRATE_STAT_CPP(UPlayerStats, EnemyAggressionFactor);

TArray<FName> UPlayerStats::GetPropertyNames()
{
	TArray<FName> PropertyNames;
	for (TFieldIterator<FProperty> Prop(StaticClass()); Prop; ++Prop) {
		const FProperty* Property = *Prop;
		// Continue if the property is not a float
		if (Property->GetClass() != FFloatProperty::StaticClass()) continue;
		PropertyNames.Add(Property->GetFName());
	}
	return PropertyNames;
}
