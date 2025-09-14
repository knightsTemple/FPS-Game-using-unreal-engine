// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/SProjectileBase.h"
#include "ASDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TAKANASHI_HOSHINO_API AASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()
public:
	AASDashProjectile();
protected:
	UPROPERTY(EditDefaultsOnly,Category="Teleport")
	float TeleportDistance;

	UPROPERTY(EditDefaultsOnly,Category="Teleport")
	float DetonationDelay;

	FTimerHandle TimerHandle_DelayDetonation;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;
};
