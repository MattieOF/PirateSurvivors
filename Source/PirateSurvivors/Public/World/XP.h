// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XP.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "XP")
	bool bPickedUp = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "XP")
	float Value = 0;

	int ID = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "XP")
	UStaticMeshComponent* Mesh;
};
