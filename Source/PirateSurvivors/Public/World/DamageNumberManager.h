// copyright lololol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageNumberManager.generated.h"

class UDamageNumberWidget;
class UWidgetComponent;

struct FDamageNumber
{
	UWidgetComponent* WidgetComponent;
	FVector Velocity;
	float BaseLifetime, Lifetime;
};

UCLASS()
class PIRATESURVIVORS_API ADamageNumberManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamageNumberManager();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Category = "Damage Number Manager")
	void AddDamageNumber(int Damage, FVector Location);

	TArray<FDamageNumber> DamageNumbers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Number Manager")
	TSubclassOf<UDamageNumberWidget> WidgetClass;
};
