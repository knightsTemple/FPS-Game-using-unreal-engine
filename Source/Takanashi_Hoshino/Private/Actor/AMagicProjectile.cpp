// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AMagicProjectile.h"

#include "Actions/SActionComponent.h"
#include "Actor/SGameplayFunctionLibrary.h"
#include "CADKernel/Utils/StringUtil.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include"GameFramework/ProjectileMovementComponent.h"


void AAMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor&&OtherActor!=GetInstigator())
	{
		// USAttributeComponent* AttributeComp=Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
  //       if (AttributeComp)
  //       {
	 //        AttributeComp->ApplyHealthChange(GetInstigator(),DamageAmount);
  //       	Destroy();
  //       }

		//static FGameplayTag Tag= FGameplayTag::RequestGameplayTag("Status.Parrying");
		USActionComponent* ActionComp=Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp&&ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity=-MovementComp->Velocity;//将速度向量反转，效果是将物体的运动方向反转。例如，如果物体原本向前移动，反转后它将向后移动。 

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Destroy();

			if (ActionComp&&HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(),BurningActionClass);
			}
		}
	}
}

// Sets default values
AAMagicProjectile::AAMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp=CreateDefaultSubobject<USphereComponent>("sphereComp");
	// SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	// SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// SphereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AAMagicProjectile::OnActorOverlap);
	RootComponent=SphereComp;

	EffectComp=CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp=CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed=6000.f;
	MovementComp->bRotationFollowsVelocity=true;//当物体移动时，其朝向（Rotation）会自动调整为与移动方向一致。例如，一个角色向前移动时，会自动转向面朝前进方向。
	MovementComp->bInitialVelocityInLocalSpace=true;//效果：速度向量将相对于物体自身的坐标系（而非世界坐标系）。例如，设置 X 轴正方向的速度时，物体会向自身前方移动，无论其当前朝向如何

	DamageAmount=-20.f;

	//SetReplicates(true);
}

// Called when the game starts or when spawned
void AAMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void AAMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

