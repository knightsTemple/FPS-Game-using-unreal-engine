// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged,AActor*,InstigatorActor,USAttributeComponent*,OwningComp,float, NewHealth,float,Delta);

DECLARE_DYNAMIC_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAKANASHI_HOSHINO_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,Category="Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable,Category="Attributes",meta=(DisplayName="IsAlive"))
	static bool IsActorAlive(AActor* FromActor);
	// Sets default values for this component's properties
	USAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated,Category="Attributes")//标记Replicated表示能够复制，仅支持 “服务端→客户端” 的单向同步
	float Health;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated,Category="Attributes")
	float HealthMax;

	UFUNCTION(NetMulticast,Reliable)//服务端主动调用，然后自动同步执行到所有客户端（包括发起者自己的客户端）。
	void MulticastOnHealthChanged(AActor* InstigatorActor,float NewHealth,float Delta);

public:	

	UFUNCTION(BlueprintCallable)
	inline bool IsAlive()const{
		return Health > 0.f;
	};

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatActor,float Delta);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Attributes")
	float SlowHealth;

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	bool IsFullHealth() const
	{
		return Health == GetHealthMax();
	}

	float GetHealthMax() const
	{
		return HealthMax;
	}
private:
	float SlowInterpSpd = 4.f;  // 速度可调
	
};
