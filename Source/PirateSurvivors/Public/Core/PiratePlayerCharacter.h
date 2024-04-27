// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "PirateGameModeBase.h"
#include "PirateSurvivorsCharacter.h"
#include "PiratePlayerCharacter.generated.h"

class UWeaponData;
class AWeaponFunctionality;
class AXP;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputAction;

UCLASS()
class PIRATESURVIVORS_API APiratePlayerCharacter : public APirateSurvivorsCharacter
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

	UFUNCTION()
	void OnPickupRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								   const FHitResult& SweepResult);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AXP*> XPBeingPickedUp;

protected:
	virtual void BeginPlay() override;

	void FindPlayerState();

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

	UFUNCTION()
	float GetArmour(FDamageInstance DamageEvent);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UPlayerMappableInputConfig* InputMappingConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInvertVertMovement = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInvertHorizontalMovement = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* PickupRange;

	UPROPERTY(BlueprintReadOnly, Category = "Pirate Player Character")
	APiratePlayerState* PiratePlayerState;
};
