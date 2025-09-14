// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionComponent.h"
#include "Actions/SAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Takanashi_Hoshino/Takanashi_Hoshino.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true); //设置组件默认被复制
}

// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("DefaultActions.Num() = %d on %s"), DefaultActions.Num(), *GetName());


	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(),ActionClass);
		}
	} 

}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugString=GetNameSafe(GetOwner())+":"+ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::White, DebugString);

	// for (USAction* Action : Actions)
	// {
	// 	FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
	// 	FString DebugString = FString::Printf(TEXT("[%s] Action : %s:IsRunning: %s, Outer: %s"),
	// 		*GetNameSafe(GetOwner()),
	// 		*GetNameSafe(Action),
	// 		Action->IsRunning() ? TEXT("true") : TEXT("false"),
	// 		*GetNameSafe(Action->GetOuter()));
	// 	LogOnScreen(GetOwner(), DebugString, TextColor, 0.1f);
	// }
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!GetOwner()->HasAuthority())
	{
		// 客户端禁止调用 AddAction（防止 DefaultActions 空）
		UE_LOG(LogTemp, Warning, TEXT("Client should not call AddAction directly on %s"), *GetName());
		return;
	}

	if (!ActionClass || !*ActionClass)
	{
		UE_LOG(LogTemp, Error, TEXT("AddAction: Invalid or null ActionClass on %s"), *GetName());
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if (!ensure(NewAction)) return;

	NewAction->SetFlags(RF_Public | RF_Transactional);
	NewAction->Initialize(this);

	Actions.Add(NewAction);

	if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
	{
		NewAction->StartAction(Instigator);
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	// 客户端发起 RPC 请求给服务器
	if (!GetOwner()->HasAuthority())
	{
		ServerStartAction(Instigator, ActionName);
		return true;
	}

	// 服务端执行逻辑
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator)) continue;
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action&&Action->ActionName==ActionName)
		{
			if (Action->IsRunning())
			{
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
                   Action->StopAction(Instigator);
			       return true;
			}
			
		}
	}
	return false;
}

void USActionComponent::RemoveAction(USAction* Action)
{
	if (!ensure(Action&&!Action->IsRunning()))
	{
		//如果动作正在运行，则不能移除
		return;
	}
	Actions.Remove(Action);
}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass) const
{
	for (USAction* Action : Actions)
	{
		if (Action&&Action->IsA(ActionClass)) //检查Action是否是ActionClass的子类或ActionClass的实例
		{
			return Action;
		}
	}
	return nullptr; //如果没有找到，则返回nullptr
}



void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}


//处理子对象的复制,对于动态创建的子对象（如代码中 Actions 数组里的 USAction 实例，可能是运行时动态添加的），默认实现不会主动将它们加入网络复制流程!!!!!!
bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction*Action:Actions)
	{
		if (Action)
		{
			WroteSomething|= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);//将子对象 Action 的数据写入复制数据包（Bunch），
			//并通过网络通道（Channel）发送
		}
	}
	return WroteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}
