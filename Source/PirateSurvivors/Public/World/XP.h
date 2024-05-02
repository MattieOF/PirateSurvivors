// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XP.generated.h"

class APiratePlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickedUp, APiratePlayerCharacter*, Player);

// XP is a collectible item that can be picked up by the player to increase their XP.
// Don't create XP directly in the world, use the XPManager to spawn them.
// XP is not directly replicated, as there are too many of them to replicate individually.
// Instead, the XPManager is responsible for replicating creation, destruction and pickup of XP.
UCLASS()
class PIRATESURVIVORS_API AXP : public AActor
{
	GENERATED_BODY()

public:
	AXP();

	UFUNCTION(BlueprintCallable, Category = "XP")
	void Hide();

	virtual void Tick(float DeltaSeconds) override;

	void SetPhysics(FVector Velocity);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "XP")
	bool bPickedUp = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "XP")
	float Value = 0;

	UPROPERTY(VisibleAnywhere)
	int ID = 0;

	float PickupDelay = 0;
	bool bCanBePickedUp = true;
	
	bool bPhysicsEnabled = false;

	UPROPERTY(BlueprintAssignable, Category = "XP")
	FOnPickedUp OnPickedUp;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "XP")
	UStaticMeshComponent* Mesh;
};
