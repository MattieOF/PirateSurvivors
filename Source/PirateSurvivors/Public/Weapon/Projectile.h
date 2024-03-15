// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class PIRATESURVIVORS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void Initialise(class UWeaponStats* WeaponStats, class UProjectileData* NewData);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SetDirection(const FVector& NewDirection);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileData* Data;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float TimeUntilDeath = -1;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Projectile")
	void Multicast_OnProjectileDeath();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void OnProjectileDeath();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;
};
