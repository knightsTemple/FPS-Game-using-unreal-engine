// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/SUserDefaultWidget.h"
#include "Actor/SWorldUserWidget.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAKANASHI_HOSHINO_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

	void PrimaryInteract();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(Server,Reliable)//Reliable（可靠）：确保数据包最终送达，未收到确认会重发。Unreliable（不可靠）：不保证送达，丢包不重试
	void ServiceInteract(AActor* InFocus);
	
	void FindBestInteractable();

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly,Category="Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly,Category="Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly,Category="Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USUserDefaultWidget* DefaultWidgetInstance;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
