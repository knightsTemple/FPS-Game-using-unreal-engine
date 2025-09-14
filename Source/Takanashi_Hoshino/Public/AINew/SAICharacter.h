// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/SUserDefaultWidget.h"
#include "Actor/SWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USActionComponent;
class USAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class TAKANASHI_HOSHINO_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	UPROPERTY(EditAnywhere,Category="Component")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,category="Components")
	TObjectPtr<USAttributeComponent>AttributeComp;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UUserWidget>HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	TObjectPtr<USActionComponent>ActionComp;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="UI")
	TSubclassOf<USUserDefaultWidget>SpottedWidgetClass;

	UPROPERTY()
	USUserDefaultWidget* NewWidget;

	USWorldUserWidget* ActiveHealthBarWidget;
	// UPROPERTY(VisibleAnywhere,Category="Effect")
	// FName TimeToHitParamName;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);

	AActor* GetTargetActor() const;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Unreliable)
	void MuticastPawnSeen();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
