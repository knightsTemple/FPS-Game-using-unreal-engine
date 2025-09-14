// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "AI/BlackBoard_Keys.h"
#include "AI/NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UFindPlayerLocation::UFindPlayerLocation()
{
	NodeName=TEXT("Find Player Location");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//获取玩家和NPC的控制器
	ACharacter *const player=UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	auto const Cont=Cast<ANPC_AIController>(OwnerComp.GetAIOwner());

	//获取玩家位置
	FVector const player_location=player->GetActorLocation();
	if (search_random)
	{
		FNavLocation loc;
		UNavigationSystemV1 *const NavSys=UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSys->GetRandomPointInNavigableRadius(player_location,search_radius,loc,nullptr))
		{
			Cont->get_blackboard()->SetValueAsVector(bb_keys::TargetLocation,loc.Location);
		}
	}
	else
	{
		Cont->get_blackboard()->SetValueAsVector(bb_keys::TargetLocation,player_location);
	}
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
