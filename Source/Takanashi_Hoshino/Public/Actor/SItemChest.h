// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SGameplayInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class TAKANASHI_HOSHINO_API ASItemChest : public AActor,public ISGameplayInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
	void Interact_Implementation(APawn* InstigatorPawn);

	void OnActorLoaded_Implementation() override;

public:
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened",BlueprintReadOnly,SaveGame)
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
