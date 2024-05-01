// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponFunctionality.h"
#include "BasicMeleeWeapon.generated.h"

class UBoxComponent;

UCLASS()
class PIRATESURVIVORS_API ABasicMeleeWeapon : public AWeaponFunctionality
{
	GENERATED_BODY()

public:
	ABasicMeleeWeapon();

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void OnFire_Implementation() override;

	virtual void OnInitialise_Implementation() override;

protected:
	UFUNCTION()
	void OnRangeChanged(float _);
	
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_OnSwing(FVector Direction);
	
	void InitialiseFromStats() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Weapon")
	UStaticMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Weapon")
	UBoxComponent* WeaponCollision = nullptr;

	FVector LastDir = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Weapon")
	bool bAnimating = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Weapon")
	float AnimationProgress = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Weapon")
	float AnimationTime = .25f;
};
