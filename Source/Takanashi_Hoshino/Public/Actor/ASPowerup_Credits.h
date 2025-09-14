// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASPowerup.h"
#include "GameFramework/Actor.h"
#include "Interface/SGameplayInterface.h"
#include "ASPowerup_Credits.generated.h"

UCLASS()
class TAKANASHI_HOSHINO_API AASPowerup_Credits : public AASPowerup
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASPowerup_Credits();

	void Interact_Implementation(APawn* InstigatorPawn);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int CreditAmount;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
