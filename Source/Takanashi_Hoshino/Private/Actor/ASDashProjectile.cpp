// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ASDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AASDashProjectile::AASDashProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeleportDistance = 0.2f;
	DetonationDelay = 0.2f;

	MovementComp->InitialSpeed = 6000.f;
}

void AASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// 设置定时器，延迟执行爆炸
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle_DelayDetonation, this, &AASDashProjectile::Explode, DetonationDelay);
}

void AASDashProjectile::Explode_Implementation()
{
	//GetWorldTimerManager().ClearTimer(TimerHandle_DelayDetonation);
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &AASDashProjectile::TeleportInstigator, TeleportDistance);
}

void AASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);//将 Actor（游戏中的实体对象）立即移动到指定位置。这个函数绕过了物理模拟和移动组件的正常移动逻辑，实现瞬间传送效果。
	}
	
}

