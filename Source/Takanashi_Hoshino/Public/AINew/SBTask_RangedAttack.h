// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class TAKANASHI_HOSHINO_API USBTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass; // 弹道类

	UPROPERTY(EditAnywhere,Category="AI")
	float MaxBulletSpread;

public:
	USBTask_RangedAttack();
};
