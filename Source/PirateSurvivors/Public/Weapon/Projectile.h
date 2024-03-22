// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class APirateSurvivorsCharacter;
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
	virtual void Initialise(APirateSurvivorsCharacter* NewOwner, class UWeaponStats* WeaponStats, class UProjectileData* NewData);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void FireInDirection(const FVector& NewDirection);
	
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", Replicated)
	UProjectileData* Data;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	APirateSurvivorsCharacter* OwningCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float TimeUntilDeath = -1;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Projectile")
	void Multicast_OnProjectileDeath();

	// This function should perform cosmetic actions, such as spawning particles or sounds.
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void OnProjectileDeath();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void ProjectileHitNonCharacter(AActor* Other);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void ProjectileHitCharacter(APirateSurvivorsCharacter* Other);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bDestroyOnHit = true;
	
	UPROPERTY(BlueprintReadOnly, Category = "Projectile", Replicated)
	float ProjectileSpeed = 0;

	// Not replicated, should only be used on the server.
	UPROPERTY(BlueprintReadWrite, Category = "Projectile")
	float ProjectileDamage = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile", Replicated)
	FVector ProjectileDirection;

	void SetupFromData();
	
	virtual void PostNetInit() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
