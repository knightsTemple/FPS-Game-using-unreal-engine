// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NPC_AIController.generated.h"

/**
 * 
 */
UCLASS()
class TAKANASHI_HOSHINO_API ANPC_AIController : public AAIController
{
	GENERATED_BODY()
public:
	ANPC_AIController();
	void BeginPlay() override;
	void OnPossess(APawn*const InPawn) override;
	class UBlackboardComponent *get_blackboard()const;
private:
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="AI",Meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBehaviorTreeComponent>behavior_tree_component;
	
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="AI",Meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBehaviorTree>btree;

	TObjectPtr<UBlackboardComponent>blackboard;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight>sight_config;

	UFUNCTION()
	void on_updated(TArray<AActor*>const&updated_actors);

	void setup_perception_system();

	void on_target_detected(AActor*actor,FAIStimulus const stimulus);
};
