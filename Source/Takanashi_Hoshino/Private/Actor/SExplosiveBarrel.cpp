// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent=MeshComp;
	MeshComp->SetSimulatePhysics(true);

	ForceComp=CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	ForceComp->SetupAttachment(MeshComp);
	//决定力场影响范围的球体半径。
	ForceComp->Radius=750.f;
	//控制施加力的大小，正值为推力，负值为吸力。
	ForceComp->ImpulseStrength=2500.f;
	//true：直接改变物体速度（推荐用于游戏性），效果与质量无关。false：基于牛顿第二定律（F=ma）计算加速度，质量越大的物体受影响越小。
	ForceComp->bImpulseVelChange=true;
	//表示只对 "动态物理对象" 生效（如可破坏的箱子、武器）
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::PostInitializeComponents()//在 Actor 的所有组件（如 Mesh、Collision、Movement 等）完成初始化后调用，当一个组件需要引用或配置另一个组件时。
{
	Super::PostInitializeComponents();
	//MeshComp->OnComponentHit.AddDynamic(this,&ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	UE_LOG(LogTemp,Warning,TEXT("HitComponent:%s,at game time:%f"),*HitComponent->GetName(),GetWorld()->GetTimeSeconds());
	
	DrawDebugString(GetWorld(),Hit.ImpactPoint,FString::Printf(TEXT("HitLocation:%s"),*Hit.ImpactPoint.ToString()),nullptr,FColor::Green,5.f,true);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

