// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ASPowerup.h"

#include "Character/Hoshino.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AASPowerup::AASPowerup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp=CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionProfileName("PowerUp");
	RootComponent=SphereComp;
	SphereComp->OnComponentBeginOverlap.AddDynamic(
		this, &AASPowerup::OnSphereOverlap);

	RespawnTime=10.0f;

	SetReplicates(true);//能够在网络上同步，启用后，该 Actor 会在服务端保持一个 “权威实例”，并可以将自身的关键信息（如位置、旋转、标记为 Replicated 的变量）复制到客户端。
	//不启用（默认 false）时，该 Actor 仅存在于生成它的端（服务端或客户端），不会在网络中同步。
}

// Called when the game starts or when spawned
void AASPowerup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AASPowerup::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void AASPowerup::ShowPowerup()
{
	SetPowerupState(true);
}

void AASPowerup::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AASPowerup::ShowPowerup, RespawnTime);
}

void AASPowerup::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

// Called every frame
void AASPowerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}

void AASPowerup::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<AHoshino>(OtherActor))
	{
		return;
	}

	Interact_Implementation(Cast<APawn>(OtherActor));
	HideAndCooldownPowerup();
}

void AASPowerup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AASPowerup, bIsActive);
}
