// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Change, float, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHurt, float, Damage, float, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealed, float, HealAmount, float, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PIRATESURVIVORS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
								FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ChangeHP(const float Amount);
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHP(const float NewHP);
	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE void Hurt(const float Damage);
	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE void Heal(const float Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE float GetHealth() const { return Health; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE bool  IsOverhealEnabled() const { return bEnableOverheal; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetOverhealEnabled(const bool bEnable, const bool bClampHP = true);
	
	// How many seconds between Health -= OverhealDecayMagnitude
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	float OverhealDecayRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	float OverhealDecayMagnitude = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float CurrentOverhealDecayDelay = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	float Health = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	bool bEnableOverheal = false;

	UPROPERTY(BlueprintAssignable)
	FOnHealed OnHealed;
	UPROPERTY(BlueprintAssignable)
	FOnHurt OnHurt;
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;
};
