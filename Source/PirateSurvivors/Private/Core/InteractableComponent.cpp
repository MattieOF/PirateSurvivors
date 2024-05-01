// copyright lololol

#include "Core/InteractableComponent.h"

#include "Components/BoxComponent.h"
#include "Core/PiratePlayerState.h"
#include "Net/UnrealNetwork.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	InteractionRange = CreateDefaultSubobject<UBoxComponent>(TEXT("Range"));
	InteractionRange->SetupAttachment(this);
}

void UInteractableComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetIsReplicated(true);
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

void UInteractableComponent::BeginInteract(APiratePlayerState* Player)
{
	if (InteractingPlayer)
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
	SetActive(true);
}

void UInteractableComponent::DisableInteraction()
{
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
