// Fill out your copyright notice in the Description page of Project Settings.


#include "AINew/SBTService_CheckAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//确认ai和Actor之间的距离
	UBlackboardComponent* BlackboardComp=OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		AActor* TargetActor=Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* Mycontroller=OwnerComp.GetAIOwner();
			if (Mycontroller)
			{
				APawn* AIPawn=Mycontroller->GetPawn();
				if (AIPawn)
				{
					float Distance=FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bwithRange=Distance<2000.0f; //设置攻击范围为2000.0f
					bool bHasLOS=false;
					if (bwithRange)
					{
						bHasLOS=Mycontroller->LineOfSightTo(TargetActor); //检查视线是否没有被阻挡
					}
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName,(bwithRange&& bHasLOS));
				}
			}
		}
	}
}
