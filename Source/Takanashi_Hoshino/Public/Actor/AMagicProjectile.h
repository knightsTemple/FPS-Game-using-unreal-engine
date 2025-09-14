// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Actions/SActionEffect.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "AMagicProjectile.generated.h"

UCLASS()
class TAKANASHI_HOSHINO_API AAMagicProjectile : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Sets default values for this actor's properties
	AAMagicProjectile();

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<USphereComponent>SphereComp;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UParticleSystemComponent>EffectComp;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent>MovementComp;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DamageAmount ;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag ParryTag;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TSubclassOf<USActionEffect>BurningActionClass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
