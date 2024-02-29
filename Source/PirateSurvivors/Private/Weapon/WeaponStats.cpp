// copyright lololol

#include "Weapon/WeaponStats.h"

TArray<FName> UWeaponStats::GetPropertyNames()
{
	TArray<FName> PropertyNames;
	for( TFieldIterator<FProperty> Prop(StaticClass()); Prop; ++Prop) {
		const FProperty* Property = *Prop;
		PropertyNames.Add(Property->GetFName());
	}
	return PropertyNames;
}
