// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "PirateGameModeBase.h"
#include "PirateSurvivorsCharacter.h"
#include "PiratePlayerCharacter.generated.h"

// Forward decls
class UWeaponData;
class AWeaponFunctionality;
class AXP;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilled, FDamageInstance, FinalBlow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRevived, APiratePlayerCharacter*, Reviver);

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
	
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                           const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AXP*> XPBeingPickedUp;

	UFUNCTION(BlueprintCallable, Category = "Pirate Player Character")
	void OnKilled();
	UFUNCTION(BlueprintCallable, Category = "Pirate Player Character")
	void OnRevived(APiratePlayerCharacter* Reviver);

	UPROPERTY(BlueprintAssignable)
	FOnKilled Killed;
	UPROPERTY(BlueprintAssignable)
	FOnRevived Revived;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractAction;

	void OnMove(const FInputActionValue& ActionValue);
	void OnJump();
	void OnMouseLook(const FInputActionValue& ActionValue);
	void OnFire(bool bHeld);
	void OnInteract(bool bPressed);

	UFUNCTION(Server, Reliable)
	void Server_OnInteract(bool bPressed);

	void OnInteractImpl(bool bPressed);
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pirate Player Character")
	float DownSpeedMultiplier = 0.08f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pirate Player Character")
	bool bIsDown = false;
};
