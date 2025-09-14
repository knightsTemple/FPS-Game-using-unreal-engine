// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ASPowerup.h"
#include "MyASPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class TAKANASHI_HOSHINO_API AMyASPowerup_HealthPotion : public AASPowerup
{
	GENERATED_BODY()
public:
	/** 构造函数：创建 MeshComp 并做基础配置 */
	AMyASPowerup_HealthPotion();

	/** 实现 ISGameplayInterface，角色交互时被调用 */
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	/* -------- 组件区域 -------- */

	/** 可视网格（碰撞禁用，仅用 SphereComp 处理交互） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	/* -------- 配置数据 -------- */

	/** 本次可恢复的生命值（或者百分比都行，看 AttributeComponent 实现） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Powerup")
	float CreditCost = 50.f;      // 默认 +50 HP
};
