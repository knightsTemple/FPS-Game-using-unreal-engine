// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "HPInterface.h"
#include "Camera/CameraComponent.h"
#include  "Actor/PickUp.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "Hoshino.generated.h"

class USActionComponent;
class USAttributeComponent;
class USInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
UCLASS()
class TAKANASHI_HOSHINO_API AHoshino : public ACharacter,public IHPInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHoshino();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float JumpVelocity;

	UPROPERTY(EditAnywhere)
	TObjectPtr<APickUp>CurrentItem;

	bool bCanMove;
	bool bHoldingItem;
	bool bIsInspecting;
	bool bIsBlackHole_Find;

	float PitchMax;
	float PitchMin;
	float CurrentPressTime;

	FVector HoldingComp;
	FRotator LastRotator;

	FVector start;
	FVector ForwardVector;
	FVector End;

	FHitResult Hit;
	FComponentQueryParams DefaultComponentQueryParams;
	FCollisionResponseParams DefaultResponseParams;

	void ToggleMovement();
	void ToggleItemPickUp();

	virtual void TakeDamage_Implementation(float Damage) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionValue&Value);
	void Look(const FInputActionValue&Value);
	void Jumping();
	void onAction();
	void OnInspect();
	void OnInspectReleased();
	void Interact();
	void BlackHoleAttack();
	void SprintStart();
	void SprintStop();
	
	UPROPERTY(VisibleAnywhere,Category="Attack")
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,category="Components")
	TObjectPtr<USAttributeComponent>AttributeComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	FName HandSocketName;

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=camera,meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent>PlayerCamera;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=camera,meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent>CameraBoom;
	
	
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext>DefaultMappingContext;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction>MoveAction;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction>LookAction;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction>JumpAction;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction>Action;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction>Inspect;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction>PrimaryAttack;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction>PrimaryInteraction;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction>BlackHoleAttackAction;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction>Sprint;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent>HoldingComponent;

	TObjectPtr<UAIPerceptionStimuliSourceComponent>Stimulus;  //表示对象可以成为AI的感知源

	void setup_stimulus();

	void Exit_game();

	void primaryAttack();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetBiggerView();

	virtual FVector GetPawnViewLocation() const override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)//游戏运行时可以通过控制台命令调用
	void HealSelf(float Amount=100);
};







