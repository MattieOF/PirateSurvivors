// copyright lololol

#include "Core/PirateSurvivorsCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

APirateSurvivorsCharacter::APirateSurvivorsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	
	// Setup rotation settings
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}
