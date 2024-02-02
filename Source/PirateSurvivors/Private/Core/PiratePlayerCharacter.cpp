// copyright lololol

#include "Core/PiratePlayerCharacter.h"

#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PirateLog.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APiratePlayerCharacter::APiratePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Setup rotation settings
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Default movement settings (copied from TP template)
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Setup boom
	// Boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Boom"));
	// Boom->SetupAttachment(RootComponent);
	// Boom->TargetArmLength = 400;
	// Boom->bUsePawnControlRotation = true;

	// Setup camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent(), USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void APiratePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddPlayerMappableConfig(InputMappingConfig);
	}
}

void APiratePlayerCharacter::OnMove(const FInputActionValue& ActionValue)
{
	Move(ActionValue.Get<FVector2D>());
}

void APiratePlayerCharacter::OnJump()
{
	DoJump();
}

void APiratePlayerCharacter::OnMouseLook(const FInputActionValue& ActionValue)
{
	Look(ActionValue.Get<FVector2D>());
}

void APiratePlayerCharacter::OnFire(bool bHeld)
{
	Fire(bHeld);
}

void APiratePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Camera->SetWorldLocation(GetActorLocation() + CameraOffset);
	Camera->SetWorldRotation(CameraRotation);
}

void APiratePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,      ETriggerEvent::Triggered, this, &APiratePlayerCharacter::OnMove);
		EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &APiratePlayerCharacter::OnMouseLook);
		EnhancedInputComponent->BindAction(JumpAction,      ETriggerEvent::Triggered, this, &APiratePlayerCharacter::OnJump);
		EnhancedInputComponent->BindAction(FireAction,      ETriggerEvent::Started,   this, &APiratePlayerCharacter::OnFire, false);
		EnhancedInputComponent->BindAction(FireAction,      ETriggerEvent::Triggered, this, &APiratePlayerCharacter::OnFire, true);
	} else
	{
		PIRATE_LOG_ERROR("Player controller has non-enhanced input component!");
	}
}

void APiratePlayerCharacter::Move(FVector2D Input)
{
	AddMovementInput(FVector(Input, 0));
}

void APiratePlayerCharacter::Look(FVector2D Input)
{
	// No looking in this game
}

void APiratePlayerCharacter::DoJump()
{
	Jump();
}

void APiratePlayerCharacter::Fire(bool bHeld)
{
	// Probably unused
}
