// copyright lololol

#include "Core/PirateSurvivorsCharacter.h"

APirateSurvivorsCharacter::APirateSurvivorsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}
