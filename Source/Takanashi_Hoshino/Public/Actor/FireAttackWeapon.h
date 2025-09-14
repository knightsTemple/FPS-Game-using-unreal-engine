// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "FireAttackWeapon.generated.h"

UCLASS()
class TAKANASHI_HOSHINO_API AFireAttackWeapon : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// Sets default values for this actor's properties
	AFireAttackWeapon();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Settings")
	float RotationSpeed=180.f;

	// 射击间隔
	UPROPERTY(EditAnywhere, Category = "Weapon Settings")
	float FireRate = 1.0f;

	UPROPERTY(EditAnywhere,Category="Weapon Settings")
	TSubclassOf<AActor>ProjectileClass;
    

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnProjectile();
private:
	FTimerHandle TimerHandle;

	FVector Location;

	FRotator rotator;

	bool CanRotator;

	FRotator TargetRotator;

	AActor* TargetActor;

	FVector TargetLocation;

	void GetCurrentLocation();

	// 射击计时器
	float TimeSinceLastFire;

	// 发射子弹
	void FireProjectile();
};

