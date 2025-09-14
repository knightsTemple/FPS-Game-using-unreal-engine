// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASPowerup.h"
#include "ASPowerup_Action.generated.h"

class USAction;
/**
 * 
 */
UCLASS()
class TAKANASHI_HOSHINO_API AASPowerup_Action : public AASPowerup
{
	GENERATED_BODY()

public:

	AASPowerup_Action();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerup")
	TSubclassOf<USAction> ActionToGrant;
};
