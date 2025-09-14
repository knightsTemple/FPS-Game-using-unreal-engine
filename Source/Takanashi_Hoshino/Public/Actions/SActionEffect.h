// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class TAKANASHI_HOSHINO_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	USActionEffect();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
    float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent,Category="Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
};
