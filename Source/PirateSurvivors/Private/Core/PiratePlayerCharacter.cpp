﻿// copyright lololol

#include "Core/PiratePlayerCharacter.h"

#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PirateLog.h"
#include "PirateSurvivors.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Core/InteractableComponent.h"
#include "Core/PirateGameState.h"
#include "Core/PiratePlayerState.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "World/XP.h"
#include "World/XPManager.h"

APiratePlayerCharacter::APiratePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetUpdateFrequency = 10;

	// Default movement settings (copied from TP template)
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_Player);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APiratePlayerCharacter::OnCapsuleBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APiratePlayerCharacter::OnCapsuleEndOverlap);

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

	// Setup health component
	HealthComponent->bRetainDamageEvents = true;
	HealthComponent->OnDeath.AddDynamic(this, &APiratePlayerCharacter::OnKilled);

	// Setup revive interaction
	ReviveInteraction = CreateDefaultSubobject<UInteractableComponent>(TEXT("ReviveInteraction"));
	ReviveInteraction->SetupAttachment(GetCapsuleComponent());
	ReviveInteraction->Multicast_SetProperties(FText::FromString("Revive"), true, 3);
	ReviveInteraction->OnInteract.AddDynamic(this, &APiratePlayerCharacter::OnReviveInteract);
}

void APiratePlayerCharacter::OnReviveInteract(APiratePlayerState* Interactor)
{
	if (!bIsDown)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Somehow revived while not down?");
		return;
	}

	Multicast_OnRevived(Interactor->GetPiratePawn());
}

void APiratePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ReviveInteraction->DisableInteraction();
	FindPlayerState();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddPlayerMappableConfig(InputMappingConfig);
	}
}

void APiratePlayerCharacter::FindPlayerState()
{
	PiratePlayerState = Cast<APiratePlayerState>(GetPlayerState());
	if (!PiratePlayerState)
	{
		// If we can't find the player state, try again next tick
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APiratePlayerCharacter::FindPlayerState);
	}
	else
	{
		if (HasAuthority())
			HealthComponent->ArmourGetter.BindDynamic(this, &APiratePlayerCharacter::GetArmour);
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
	if (bIsDown)
		return;
	
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

void APiratePlayerCharacter::OnInteract(bool bPressed)
{
	OnInteractImpl(bPressed);
	Server_OnInteract(bPressed);
}

void APiratePlayerCharacter::Server_OnInteract_Implementation(bool bPressed)
{
	OnInteractImpl(bPressed);
}

void APiratePlayerCharacter::OnInteractImpl(bool bPressed)
{
	if (bIsDown)
		return;
	
	if (bPressed
		&& PiratePlayerState->CurrentInteractable
		&& !PiratePlayerState->CurrentInteractable->HasInteractingPlayer())
	{
		PiratePlayerState->CurrentInteractable->BeginInteract(PiratePlayerState);
	}
	else if (!bPressed
		&& PiratePlayerState->CurrentInteractable
		&& PiratePlayerState->CurrentInteractable->GetInteractingPlayer() == PiratePlayerState)
	{
		PiratePlayerState->CurrentInteractable->EndInteract(PiratePlayerState);
	}
}

// Can't be const because it's a dynamic delegate
// ReSharper disable once CppMemberFunctionMayBeConst
float APiratePlayerCharacter::GetArmour(FDamageInstance DamageEvent)
{
	if (PiratePlayerState && PiratePlayerState->PlayerStats)
		return PiratePlayerState->PlayerStats->Armor;
	return 0;
}

void APiratePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Camera->SetWorldLocation(GetActorLocation() + CameraOffset);
	Camera->SetWorldRotation(CameraRotation);

	if (HasAuthority())
	{
		for (int Index = XPToConsider.Num() - 1; Index >= 0; Index--)
		{
			if (!XPToConsider[Index])
			{
				XPToConsider.RemoveAt(Index);
				continue;
			}

			if (XPToConsider[Index]->bCanBePickedUp)
			{
				if (XPToConsider[Index]->bPickedUp) return;
				APirateGameState::GetPirateGameState(GetWorld())->GetXPManager()->Multicast_PickupXP(this, XPToConsider[Index]->ID);
				XPToConsider.RemoveAt(Index);
			}
		}
	}
	
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
		XPBeingPickedUp[Index]->SetActorLocation(
			XPBeingPickedUp[Index]->GetActorLocation() + DirNormalised
				* (800 // * (GetCharacterMovement()->MaxWalkSpeed
				* DeltaTime));

		// Then, if we're close enough, pick it up. If we're the server, tell all clients it's been picked up
		// TODO: Might be better if XP is added on pickup instead of when it's close enough
		// Since pickup serverside, there's no worry about cheating
		// Will have to think of a way so clients can recover from picking up XP that the server didn't agree with
		if (Direction.Size() < 70)
		{
			APiratePlayerState* PS = Cast<APiratePlayerState>(GetPlayerState());
			if (!PS)
				continue;
			PS->AddXP(XPBeingPickedUp[Index]->Value);
			XPBeingPickedUp[Index]->OnPickedUp.Broadcast(this);
			APirateGameState::GetPirateGameState(GetWorld())->GetXPManager()->DestroyXP(XPBeingPickedUp[Index]->ID);	
			XPBeingPickedUp[Index]->Hide();
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
		EnhancedInputComponent->BindAction(InteractAction,  ETriggerEvent::Started,   this, &APiratePlayerCharacter::OnInteract, true);
		EnhancedInputComponent->BindAction(InteractAction,  ETriggerEvent::Completed, this, &APiratePlayerCharacter::OnInteract, false);
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
	// Without this check, everything fucking breaks in weird ways, and I'm too tired to reason about why.
	// It's a good check either way, but it breaks in weird ways without it, and I don't really get it.
	if (bIsDown)
		return;
	
	if (AXP* XPActor = Cast<AXP>(OtherActor))
	{
		if (HasAuthority())
		{
			if (XPActor->bCanBePickedUp)
			{
				if (XPActor->bPickedUp) return;
				APirateGameState::GetPirateGameState(GetWorld())->GetXPManager()->Multicast_PickupXP(this, XPActor->ID);	
			} else
			{
				if (!XPToConsider.Contains(XPActor))
					XPToConsider.Add(XPActor);
			}
		} else
		{
			// So, we have a bit of an issue where XP picked up during a specific time during player connection such that,
			// while the XP was included in the initial replication, it was too early to send the pickup RPC.
			// This means that the XP is picked up on the server, but not on the client, and since the client doesn't know,
			// it'll stay there forever.
			// One way to fix this could be to have the client occasionally ask the server about all XP IDs that it's
			// recently been near, and then the server can reply if any of them are invalid and tell the client to remove
			// them.
		}
	}
}

// Can't be const because it's a dynamic delegate
// ReSharper disable once CppMemberFunctionMayBeConst

void APiratePlayerCharacter::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!(IsLocallyControlled() || GetNetMode() != NM_Client))
		return;

	if (!PiratePlayerState)
	{
		// Happens rarely, for now we'll just ignore...
		return;
	}

	if (bIsDown)
		return;

	if (!PiratePlayerState)
	{
		// Happens sometimes, not really sure why, don't have time to find out why, so just prevent a crash.
		PIRATE_LOGC_WARN_NOLOC(GetWorld(), "In OnCapsuleBeginOverlap, but PiratePlayerState is null!");
		return;
	}
	
	if (UInteractableComponent* Interactable = Cast<UInteractableComponent>(OtherComp->GetOuter()))
	{
		if (Interactable == ReviveInteraction)
			return;
		PiratePlayerState->SetInteractable(Interactable);
	}
}

// Can't be const because it's a dynamic delegate
// ReSharper disable once CppMemberFunctionMayBeConst

void APiratePlayerCharacter::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!(IsLocallyControlled() || GetNetMode() != NM_Client))
		return;
	
	if (bIsDown)
		return;
	
	if (!PiratePlayerState)
	{
		// Happens sometimes, not really sure why, don't have time to find out why, so just prevent a crash.
		PIRATE_LOGC_WARN_NOLOC(GetWorld(), "In OnCapsuleBeginOverlap, but PiratePlayerState is null!");
		return;
	}
	
	if (UInteractableComponent* Interactable = Cast<UInteractableComponent>(OtherComp->GetOuter()))
	{
		if (Interactable == PiratePlayerState->CurrentInteractable)
		{
			PiratePlayerState->SetInteractable(nullptr);
			Interactable->EndInteract(PiratePlayerState);
		}
	}
}

void APiratePlayerCharacter::OnKilled()
{
	if (bIsDown)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Somehow killed while already down?");
		return;
	}

	if (!PiratePlayerState)
	{
		PIRATE_LOGC_ERROR(GetWorld(), "Somehow killed before player state initialised?");
		return;
	}
	
	Killed.Broadcast(HealthComponent->DamageHistory.IsEmpty() ? FDamageInstance{} : HealthComponent->DamageHistory.Last());
	GetCharacterMovement()->MaxWalkSpeed *= DownSpeedMultiplier;
	bIsDown = true;
	ReviveInteraction->EnableInteraction();
	if (PiratePlayerState->CurrentInteractable)
	{
		PiratePlayerState->CurrentInteractable->EndInteract(PiratePlayerState);
		PiratePlayerState->SetInteractable(nullptr);
	}

	if (HasAuthority())
	{
		APirateGameState* GS = APirateGameState::GetPirateGameState(GetWorld());
		if (!GS)
			return;
		GS->OnPlayerDied();
	}
}

void APiratePlayerCharacter::Multicast_OnRevived_Implementation(APiratePlayerCharacter* Reviver)
{
	PIRATE_LOGC(GetWorld(), "Revived %s, by %s", *GetName(), *Reviver->GetName());
	OnRevived(Reviver);
}

void APiratePlayerCharacter::OnRevived(APiratePlayerCharacter* Reviver)
{
	Revived.Broadcast(Reviver);
	GetCharacterMovement()->MaxWalkSpeed /= DownSpeedMultiplier;
	bIsDown = false;
	ReviveInteraction->DisableInteraction();
	HealthComponent->Revive(.2f);

	if (HasAuthority())
	{
		// Spawn some revive XP
		int Count = FMath::RandRange(10, 15);
		for (int i = 0; i < Count; i++)
		{
			APirateGameState::GetPirateGameState(GetWorld())->GetXPManager()->SpawnXPWithPhysics(
				GetActorLocation() + FVector(0, 0, 100), FMath::RandRange(1, 2), 0,
				FVector(FMath::RandRange(-200, 200), FMath::RandRange(-200, 200), FMath::RandRange(0, 200)));
		}
	}
}
