// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"Camera/CameraComponent.h"
#include "PickUp.generated.h"

UCLASS()
class TAKANASHI_HOSHINO_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();
	
	UPROPERTY(EditAnywhere)
    UStaticMeshComponent *MyMesh;
	
	UPROPERTY(EditAnywhere)
	USceneComponent *HoldingCamp;
	
	UFUNCTION()
	void RotateActor();
	
	UFUNCTION()
	void  PickUp();
	
	bool bHolding;
	bool bGravity;
	
	FRotator ControlRotation;
	TObjectPtr<ACharacter>MyCharacter;
	TObjectPtr<UCameraComponent>PlayerCamera;
	FVector ForwardVector;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
