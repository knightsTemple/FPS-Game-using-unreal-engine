// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SAttributeComponent.h"

#include "Character/SGameModeBase.h"
#include "Net/UnrealNetwork.h"

class USAttributeComponent;
static  TAutoConsoleVariable<float>CVarDamageMultiplier(TEXT("su.DamageMultiplier"),1.0f,TEXT("Damage Multiplier for testing."),ECVF_Cheat);

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* FromActor)
{
	USAttributeComponent *AttributeComp=GetAttributes(FromActor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	HealthMax=100.f;

	Health= HealthMax; //初始化Health为HealthMax\

	SlowHealth=HealthMax;

	SetIsReplicatedByDefault(true); //设置组件默认被复制
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


bool USAttributeComponent::ApplyHealthChange(AActor* InstigatActor,float Delta)
{
	if (!GetOwner()->CanBeDamaged()&&Delta<0.0f)
	{
		return false; //如果不能被伤害，则直接返回false
	}
	
	if (Delta<0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier; //应用伤害倍率
	}
	float OldHealth=Health;
	float NewHealth=FMath::Clamp(Health+Delta,0.f,HealthMax);
	float ActualDelta=NewHealth-OldHealth;
	
	if (GetOwner()->HasAuthority())
	{
		Health=NewHealth; //如果是服务器，则直接修改Health

		if (ActualDelta!=0.0f)
		{
			MulticastOnHealthChanged(InstigatActor,Health,ActualDelta);
		}

		if (ActualDelta<0.0f&&Health==0.0f) 
		{
			ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GameMode)
			{
				GameMode->OnActorKilled(GetOwner(), InstigatActor); //通知游戏模式，角色被杀死
			}
		
		}
	}

	//OnHealthChanged.Broadcast(InstigatActor,this,Health,ActualDelta);
	return ActualDelta!=0.f; //如果实际变化不为0，则返回true
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		FString::Printf(TEXT("被 %s 杀死了!"), *InstigatorActor->GetName()));
	return ApplyHealthChange(InstigatorActor,GetHealthMax());
}

void USAttributeComponent::MulticastOnHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
	float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//将Health和HealthMax标记为需要复制的属性
	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);
	//DOREPLIFETIME_CONDITION(USAttributeComponent,HealthMax,COND_InitialOnly); //HealthMax只在初始时复制一次
}