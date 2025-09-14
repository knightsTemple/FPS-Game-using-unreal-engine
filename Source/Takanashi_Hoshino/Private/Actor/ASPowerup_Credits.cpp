// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ASPowerup_Credits.h"

#include "Character/SPlayerState.h"

// Sets default values
AASPowerup_Credits::AASPowerup_Credits()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreditAmount=80;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

}

void AASPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PS->AddCredits(CreditAmount);
		HideAndCooldownPowerup();
	}
}

// Called when the game starts or when spawned
void AASPowerup_Credits::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASPowerup_Credits::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

