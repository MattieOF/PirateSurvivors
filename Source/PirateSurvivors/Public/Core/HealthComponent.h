﻿// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// Forward decls
class APirateSurvivorsCharacter;

// Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Change, float, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHurt, float, Damage, float, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealed, float, HealAmount, float, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

USTRUCT(BlueprintType)
struct FDamageInstance
{
	GENERATED_BODY()

public:
	// The description of the damage event (example: "Player's Cannonball")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Event")
	FText Description = FText::FromString("DAMAGE!!!");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Event")
	float Damage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Event")
	bool bArmorPiercing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Event")
	APirateSurvivorsCharacter* Instigator = nullptr;
};

// This delegate needs FDamageInstance to be declared, so it's down here.
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(float, FGetArmour, FDamageInstance, DamageEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PIRATESURVIVORS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
								FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(const FDamageInstance& DamageEvent);
	UFUNCTION(Category = "Health", NetMulticast, Reliable)
	void Multicast_TakeDamage(const FDamageInstance& DamageEvent);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ChangeHP(const float Amount);
	UFUNCTION(BlueprintCallable, Category = "Health", NetMulticast, Reliable)
	void Multicast_ChangeHP(const float Amount);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHP(const float NewHP, const bool bResetPrevious = true);
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Multicast_SetHP(const float NewHP, const bool bResetPrevious = true);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHP(const float NewMaxHP, const bool bClampHP = true, const bool bGiveDifference = false);
	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE void Hurt(const float Damage) { ChangeHP(-Damage); }
	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE void Heal(const float Amount) { ChangeHP(Amount); }
	
	UFUNCTION(BlueprintCallable, Category = "Health", NetMulticast, Reliable)
	void Multicast_Die();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Revive(float RevivalHPPercentage);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE float GetHealth() const { return Health; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE float GetHealthPercent() const { return Health / MaxHealth; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE bool  IsOverhealEnabled() const { return bEnableOverheal; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE bool IsDead() const { return bDead; }
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetOverhealEnabled(const bool bEnable, const bool bClampHP = true);
	
	// Adds each damage event to the DamageEvents array, so that it can be used later for damage reports, etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	bool bRetainDamageEvents = false;
	
	// How many seconds between Health -= OverhealDecayMagnitude
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	float OverhealDecayRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	float OverhealDecayMagnitude = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float CurrentOverhealDecayDelay = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	TArray<FDamageInstance> DamageHistory;

	UPROPERTY(BlueprintAssignable)
	FOnHealed OnHealed;
	UPROPERTY(BlueprintAssignable)
	FOnHurt OnHurt;
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGetArmour ArmourGetter;
	
protected:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated, ReplicatedUsing = OnRep_Health)
	float Health = 100;
	
	UFUNCTION()
	void OnRep_Health(float OldHealth);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated, ReplicatedUsing = OnRep_MaxHealth)
	float MaxHealth = 100;

	UFUNCTION()
	void OnRep_MaxHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	bool bEnableOverheal = false;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;

	float PreviousHP = 100;

	bool bDead = false;
};
