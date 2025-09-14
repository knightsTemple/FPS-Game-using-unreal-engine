// Fill out your copyright notice in the Description page of Project Settings.


#include "AINew/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("没有设置行为树!")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	

	// APawn* MyPawn=UGameplayStatics::GetPlayerPawn(this,0);
	// if (MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	//
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	// }
}
