#include "Player/PlayerStats.h"

TArray<FName> UPlayerStats::GetPropertyNames()
{
	TArray<FName> PropertyNames;
	for (TFieldIterator<FProperty> Prop(StaticClass()); Prop; ++Prop) {
		const FProperty* Property = *Prop;
		PropertyNames.Add(Property->GetFName());
	}
	return PropertyNames;
}
