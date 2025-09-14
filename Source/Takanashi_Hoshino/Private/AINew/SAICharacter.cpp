// Fill out your copyright notice in the Description page of Project Settings.


#include "AINew/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Actions/SActionComponent.h"
#include "Actor/SUserDefaultWidget.h"
#include "Actor/SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Component/SAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // AI控制器会在放置或生成时自动拥有这个角色

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore); // 忽略世界动态碰撞
	GetMesh()->SetGenerateOverlapEvents(true); // 生成重叠事件

	//TimeToHitParamName = FName("TimeOfHit"); // 用于材质参数的名称
}

// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this,&ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{   
	AAIController* AIC=Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	}
	return nullptr;
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (GetTargetActor()!=Pawn) // 如果当前目标不是被看到的Pawn
	{
		SetTargetActor(Pawn);

		MuticastPawnSeen(); // 调用多播函数，通知所有客户端
	}
	
}

void ASAICharacter::MuticastPawnSeen_Implementation()
{
	USUserDefaultWidget* Widget = CreateWidget<USUserDefaultWidget>(GetWorld(), SpottedWidgetClass);
	if (Widget)
	{
		Widget->AttachedActor = this; // 设置附加的Actor为当前角色
		Widget->AddToViewport(10); // 添加到视口，ZOrder为10
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.f)
	{
		if (InstigatorActor!=this)
		{
			SetTargetActor(InstigatorActor);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
				FString::Printf(TEXT("被 %s 攻击了!"), *InstigatorActor->GetName()));
		}

		if (ActiveHealthBarWidget==nullptr)
		{
			ActiveHealthBarWidget= CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBarWidget)
			{
				ActiveHealthBarWidget->AttachedActor = this; // 设置附加的Actor为当前角色
				ActiveHealthBarWidget->AddToViewport();
			}
		}
		
		if (NewHealth<=0.0f)
		{
			//停止BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
		
			//布娃娃
			GetMesh()->SetAllBodiesSimulatePhysics(true); // 设置角色的所有骨骼模拟物理
			GetMesh()->SetCollisionProfileName(TEXT("Ragdoll")); // 设置碰撞配置为布娃娃

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 禁用胶囊碰撞
			GetCharacterMovement()->DisableMovement(); // 禁用角色移动
			//设置lifespan
			SetLifeSpan(5.0f); // 设置角色在死亡后5秒后销毁
		}
	}
}



// Called every frame
void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (!PawnSensingComp->CouldSeePawn(Cast<APawn>(GetTargetActor())))
	// {
	// 	if (NewWidget)
	// 	{
	// 		NewWidget->RemoveFromParent(); // 从视口中移除小部件
	// 		NewWidget = nullptr; // 清空指针
	// 	}
	// }

}

// Called to bind functionality to input
void ASAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

