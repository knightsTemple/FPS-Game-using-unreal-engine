// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"

class USAttributeComponent;
/**
 * 
 */
UCLASS()
class TAKANASHI_HOSHINO_API USActionEffect_Thorns : public USAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;// 反射伤害的比例

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float Duration; // 持续时间

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float Period; // 周期性效果的间隔时间

	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

public:
	USActionEffect_Thorns();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;
};
