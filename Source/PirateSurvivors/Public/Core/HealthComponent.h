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

	void ChangeHP(const float Amount);
	void SetHP(const float NewHP);
	FORCEINLINE void Hurt(const float Damage);
	FORCEINLINE void Heal(const float Amount);

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE bool  IsOverhealEnabled() const { return bEnableOverheal; }
	
	// How many seconds between Health -= OverhealDecayMagnitude
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	float OverhealDecayRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Replicated)
	float OverhealDecayMagnitude = 1;

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
