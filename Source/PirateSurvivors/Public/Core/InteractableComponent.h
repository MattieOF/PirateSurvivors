// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

// Forward decls
class APiratePlayerState;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, APiratePlayerState*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPropertiesUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionDisabled);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PIRATESURVIVORS_API UInteractableComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UInteractableComponent();

	virtual void InitializeComponent() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable", Replicated)
	FText Name = FText::FromString("Do a thing");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable", Replicated)
	bool bRequiresHold = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable", Replicated)
	float HoldTime = 1.f;

	UFUNCTION(BlueprintCallable, Category = "Interactable", NetMulticast, Reliable)
	void Multicast_SetProperties(const FText& NewName, bool bNewRequiresHold, float NewHoldTime);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void BeginInteract(APiratePlayerState* Player);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void EndInteract(APiratePlayerState* Player);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void InteractionCompleted(APiratePlayerState* Player);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void EnableInteraction();
	
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void DisableInteraction();
	
	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FOnInteract OnInteract;

	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FOnPropertiesUpdated PropertiesUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FOnInteractionDisabled InteractionDisabled;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interactable")
	FORCEINLINE bool HasInteractingPlayer() const { return InteractingPlayer != nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interactable")
	FORCEINLINE APiratePlayerState* GetInteractingPlayer() const { return InteractingPlayer; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interactable")
	FORCEINLINE float GetInteractionProgress() { return InteractingPlayer ? 1 - (CurrentInteractionTime / HoldTime) : 0; }
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Interactable")
	FVector ColliderExtents;

	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
	float CurrentInteractionTime = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
	APiratePlayerState* InteractingPlayer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
	UBoxComponent* InteractionRange;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
