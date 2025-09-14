// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TAKANASHI_HOSHINO_API UFindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UFindPlayerLocation();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Search",Meta=(AllowPrivateAccess="true"))
	bool search_random=false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Search",Meta=(AllowPrivateAccess="true"))
	float search_radius=1500.f;
};
