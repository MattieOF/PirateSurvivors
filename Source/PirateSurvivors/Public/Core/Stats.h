// copyright lololol
#pragma once

// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", Setter = SetMaxHealth)
// float MaxHealth = 100;
//
// UFUNCTION(BlueprintCallable)
// void SetMaxHealth(float Value);
//
// UPROPERTY(BlueprintAssignable)
// FOnStatChanged OnMaxHealthChanged;

#define DECLARE_PIRATE_STAT(Name, DefaultValue, Cat) \
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = #Cat, Setter = Set##Name)\
	float Name = DefaultValue;\
	\
	UFUNCTION(BlueprintCallable)\
	void Set##Name(float Value);\
	\
	UPROPERTY(BlueprintAssignable)\
	FOnStatChanged On##Name##Changed;

#define DECLARE_PIRATE_STAT_CPP(ClassName, Name) \
	void ClassName::Set##Name(float Value)\
	{\
		Name = Value;\
		On##Name##Changed.Broadcast(Value);\
	}
