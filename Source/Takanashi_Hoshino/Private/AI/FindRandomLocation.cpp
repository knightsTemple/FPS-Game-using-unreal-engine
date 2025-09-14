// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FindRandomLocation.h"

#include "NavigationSystem.h"
#include "AI/BlackBoard_Keys.h"
#include "AI/NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UFindRandomLocation::UFindRandomLocation()
{
	NodeName=TEXT("Find Random Location");
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI controller and its NPC
	if (auto const Cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		if (auto const NPC = Cont->GetPawn())
		{
			// obtain npc location to use as an origin location
			FVector const Origin = NPC->GetActorLocation();

			// get the navigation system and generate a random location on the NavMesh
			if (UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, search_radius, Loc, nullptr))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("TargetLocation"), Loc.Location);
				}
				


				// finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}