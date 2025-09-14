// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NPC_AIController.h"

#include "AI/BlackBoard_Keys.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/Hoshino.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"


ANPC_AIController::ANPC_AIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/MyPerson/AI/NPC_BT.NPC_BT'"));
	if (obj.Succeeded())
	{
		btree = obj.Object;
	}
	behavior_tree_component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	setup_perception_system();
	
}

void ANPC_AIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(btree);  //这个是启动行为树
	behavior_tree_component->StartTree(*btree);//这个可以切换行为树
}

void ANPC_AIController::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);
	if (blackboard && btree && btree->BlackboardAsset)
	{
		blackboard->InitializeBlackboard(*btree->BlackboardAsset);
	}
	
}

UBlackboardComponent* ANPC_AIController::get_blackboard() const
{
	return blackboard;
}

void ANPC_AIController::on_updated(TArray<AActor*> const& updated_actors)
{
}

void ANPC_AIController::setup_perception_system()
{
	sight_config=CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config")); //这个就相当于设置感知的参数，比如视野，视野角度等等
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"))); //这个就是设置感知组件，用来实际执行感知任务，不止视觉，还有听觉等等
	sight_config->SightRadius=500.f;  //这个就是设置视野半径
	sight_config->LoseSightRadius=sight_config->SightRadius+50.f;  //这个就是设置失去视野半径，意思是在这个范围内，如果目标消失了，AI还是可以记住你的位置，但是如果超过这个范围，AI就会忘记你的位置
	sight_config->PeripheralVisionAngleDegrees=90.f;
	sight_config->SetMaxAge(5.f);
	sight_config->AutoSuccessRangeFromLastSeenLocation=900.f;
	sight_config->DetectionByAffiliation.bDetectEnemies=true;
	sight_config->DetectionByAffiliation.bDetectFriendlies=true;
	sight_config->DetectionByAffiliation.bDetectNeutrals=true;

	GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this,&ANPC_AIController::on_updated);
	GetPerceptionComponent()->ConfigureSense(*sight_config);
}

void ANPC_AIController::on_target_detected(AActor* actor, FAIStimulus const stimulus)
{
    
	if (auto const ch=Cast<AHoshino>(actor))
	{
		get_blackboard()->SetValueAsBool(bb_keys::can_see_player,stimulus.WasSuccessfullySensed());
	}
}
