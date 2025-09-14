// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FireAttackWeapon.h"

#include "Actor/AMagicProjectile.h"
#include "Character/Hoshino.h"
#include "Kismet/KismetMathLibrary.h"


void AFireAttackWeapon::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor && Cast<AHoshino>(OtherActor))
		{
			TargetActor = OtherActor;
			CanRotator= true;
			TimeSinceLastFire = FireRate; // 立即发射第一发子弹
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap - Weapon Activated"));
		}
	}
}

void AFireAttackWeapon::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor == TargetActor)
	{
		CanRotator= false;
		TargetActor= nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("End Overlap - Weapon Deactivated"));
	}
}
void AFireAttackWeapon::SpawnProjectile()
{
	Location=GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	rotator=UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Location);
	TargetRotator=GetActorRotation()+rotator;
	//SetActorRotation(rotator);
	
}

void AFireAttackWeapon::GetCurrentLocation()
{
	if (TargetActor)
	{
		TargetLocation=TargetActor->GetActorLocation();
	}
}

void AFireAttackWeapon::FireProjectile()
{
	if (GetWorld()&&ProjectileClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
		// 获取枪口位置和方向
		FVector spawnLocation = GetActorLocation();
		FRotator spawnRotation = GetActorRotation();

		// AAMagicProjectile* projectileTemple = NewObject<AAMagicProjectile>();
		// projectileTemple->MovementComp->InitialSpeed=500.f;
		// SpawnParams.Template= projectileTemple;
		// 生成子弹
		AAMagicProjectile* projectile = GetWorld()->SpawnActor<AAMagicProjectile>(
		ProjectileClass, spawnLocation, spawnRotation, SpawnParams);
        
		if (projectile)
		{
			// 可以在这里设置子弹的属性，如速度、伤害等
			projectile->SetOwner(this);
		}
	}
}

// Sets default values
AFireAttackWeapon::AFireAttackWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp=CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent=SphereComp;
	
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);

	SphereComp->SetSphereRadius(500);
	SphereComp->SetCollisionProfileName("OverlapAll");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AFireAttackWeapon::OnActorOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this,&AFireAttackWeapon::EndOverlap);

	CanRotator=false;

	TimeSinceLastFire=0;
	
}

// Called when the game starts or when spawned
void AFireAttackWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFireAttackWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CanRotator&&TargetActor)
	{
		GetCurrentLocation();
		FRotator currentRotation = GetActorRotation();
		FRotator desiredRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
		FRotator newRotation = FMath::RInterpTo(currentRotation, desiredRotation, DeltaTime, RotationSpeed);
		SetActorRotation(newRotation);
		// 计时并发射子弹
		TimeSinceLastFire += DeltaTime;
		if (TimeSinceLastFire >= FireRate)
		{
			FireProjectile();
			TimeSinceLastFire = 0.0f;
		}
	}
}

