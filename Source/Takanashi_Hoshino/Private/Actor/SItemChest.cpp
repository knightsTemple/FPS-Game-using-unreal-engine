// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SItemChest.h"

#include "Net/UnrealNetwork.h"


void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	//LidMesh->SetRelativeRotation(FRotator(TargetPitch,0,0));
	bLidOpened = !bLidOpened;

	OnRep_LidOpened();
}

void ASItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened()
{
	// This function is called on clients when bLidOpened is replicated
	float NewPitch = bLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(NewPitch, 0.f, 0.f));
}
// Sets default values
ASItemChest::ASItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent=BaseMesh;

	LidMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch=110;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASItemChest, bLidOpened);
}
