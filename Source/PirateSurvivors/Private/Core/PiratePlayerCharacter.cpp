// copyright lololol

#include "Core/PiratePlayerCharacter.h"

#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PirateLog.h"
#include "PirateSurvivors.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Core/PirateGameState.h"
#include "Core/PiratePlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "World/XP.h"
#include "World/XPManager.h"

APiratePlayerCharacter::APiratePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetUpdateFrequency = 10;

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

	GetCapsuleComponent()->SetCollisionObjectType(ECC_Player);

	// Setup boom
	// Boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Boom"));
	// Boom->SetupAttachment(RootComponent);
	// Boom->TargetArmLength = 400;
	// Boom->bUsePawnControlRotation = true;

	// Setup camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent(), USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Setup pickup range
	PickupRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRange"));
	PickupRange->SetupAttachment(GetCapsuleComponent());
	PickupRange->SetSphereRadius(100);
	PickupRange->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	PickupRange->OnComponentBeginOverlap.AddDynamic(this, &APiratePlayerCharacter::OnPickupRangeBeginOverlap);
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

void APiratePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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

	// Iterate backwards to allow removing elements
	for (int Index = XPBeingPickedUp.Num() - 1; Index >= 0; Index--)
	{
		if (!XPBeingPickedUp[Index])
		{
			XPBeingPickedUp.RemoveAt(Index);
			continue;
		}
		
		// First, move the XP closer to the player
		FVector Direction = GetActorLocation() - XPBeingPickedUp[Index]->GetActorLocation();
		FVector DirNormalised = Direction.GetSafeNormal();
		XPBeingPickedUp[Index]->SetActorLocation(XPBeingPickedUp[Index]->GetActorLocation() + DirNormalised * 800 * DeltaTime);

		// Then, if we're close enough, pick it up. If we're the server, tell all clients it's been picked up
		// TODO: Might be better if XP is added on pickup instead of when it's close enough
		// Since pickup serverside, there's no worry about cheating
		// Will have to think of a way so clients can recover from picking up XP that the server didn't agree with
		if (Direction.Size() < 70)
		{
			Cast<APiratePlayerState>(GetPlayerState())->AddXP(XPBeingPickedUp[Index]->Value); 
			if (HasAuthority())
				APirateGameState::GetPirateGameState(GetWorld())->GetXPManager()->Multicast_DestroyXP(XPBeingPickedUp[Index]->ID);
			XPBeingPickedUp[Index]->Destroy();
			XPBeingPickedUp.RemoveAt(Index);
		}
	}
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

void APiratePlayerCharacter::OnPickupRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	
	if (const AXP* XPActor = Cast<AXP>(OtherActor))
	{
		if (XPActor->bPickedUp) return;
		APirateGameState::GetPirateGameState(GetWorld())->GetXPManager()->Multicast_PickupXP(this, XPActor->ID);
	}
}
