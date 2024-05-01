// copyright lololol

#include "Core/InteractableComponent.h"

#include "Components/BoxComponent.h"
#include "Core/PiratePlayerCharacter.h"
#include "Core/PiratePlayerState.h"
#include "Net/UnrealNetwork.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UInteractableComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetIsReplicated(true);
	InteractionRange = NewObject<UBoxComponent>(this, TEXT("Range"));
	InteractionRange->RegisterComponent();
	InteractionRange->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	// InteractionRange->SetBoxExtent(ColliderExtents);
}

void UInteractableComponent::Multicast_SetProperties_Implementation(const FText& NewName, bool bNewRequiresHold,
                                                                    float NewHoldTime)
{
	Name = NewName;
	bRequiresHold = bNewRequiresHold;
	HoldTime = NewHoldTime;

	if (!bRequiresHold)
	{
		InteractingPlayer = nullptr;
	}

	PropertiesUpdated.Broadcast();
}

void UInteractableComponent::Multicast_SetHoldTime_Implementation(float NewHoldTime)
{
	HoldTime = NewHoldTime;
	PropertiesUpdated.Broadcast();
}

void UInteractableComponent::BeginInteract(APiratePlayerState* Player)
{
	if (InteractingPlayer)
		return;

	if (Player->GetPiratePawn()->IsDown())
		return;
	
	if (!bRequiresHold)
	{
		InteractionCompleted(Player);
	} else
	{
		InteractingPlayer = Player;
		CurrentInteractionTime = HoldTime;
	}
}

void UInteractableComponent::EndInteract(APiratePlayerState* Player)
{
	if (InteractingPlayer == Player)
	{
		InteractingPlayer = nullptr;
	}
}

void UInteractableComponent::InteractionCompleted(APiratePlayerState* Player)
{
	Player->SetInteractable(nullptr);
	OnInteract.Broadcast(Player);
}

void UInteractableComponent::EnableInteraction()
{
	InteractionRange->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActive(true);
}

void UInteractableComponent::DisableInteraction()
{
	InteractionRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractionDisabled.Broadcast();
	SetActive(false);
}

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(UInteractableComponent, Name, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(UInteractableComponent, bRequiresHold, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(UInteractableComponent, HoldTime, COND_InitialOnly);
}

void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (InteractingPlayer)
	{
		CurrentInteractionTime -= DeltaTime;
		if (CurrentInteractionTime <= 0)
		{
			InteractionCompleted(InteractingPlayer);
			EndInteract(InteractingPlayer);
		}
	}
}
