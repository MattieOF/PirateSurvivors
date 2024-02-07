﻿// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "PirateGameModeBase.h"
#include "GameFramework/Character.h"
#include "PiratePlayerCharacter.generated.h"

class AXP;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputAction;

UCLASS()
class PIRATESURVIVORS_API APiratePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APiratePlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Move(FVector2D Input);

	UFUNCTION(BlueprintCallable)
	void Look(FVector2D Input);

	UFUNCTION(BlueprintCallable)
	void DoJump();

	UFUNCTION(BlueprintCallable)
	void Fire(bool bHeld);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetCurrentXPMultiplier() const
	{
		const APirateGameModeBase* const GameMode = APirateGameModeBase::GetPirateGameMode(this);
		return Level < GameMode->XPMultipliers.Num() ? GameMode->XPMultipliers[Level] : GameMode->XPMultipliers.Last();
	}

	UFUNCTION(BlueprintCallable)
	void AddXP(float AddedXP);
	
	UFUNCTION()
	void OnPickupRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								   const FHitResult& SweepResult);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AXP*> XPBeingPickedUp;
	
protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CameraOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator CameraRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MouseMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction;

	void OnMove(const FInputActionValue& ActionValue);
	void OnJump();
	void OnMouseLook(const FInputActionValue& ActionValue);
	void OnFire(bool bHeld);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UPlayerMappableInputConfig* InputMappingConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInvertVertMovement = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInvertHorizontalMovement = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere)
	int Level = 0;
	UPROPERTY(Replicated, BlueprintReadOnly, EditAnywhere)
	float XP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* PickupRange;
};
