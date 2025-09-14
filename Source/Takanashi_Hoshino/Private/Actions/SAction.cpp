// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction.h"

#include "Net/UnrealNetwork.h"
#include "Takanashi_Hoshino/Takanashi_Hoshino.h"


void USAction::Initialize(USActionComponent* NewActionComponent)
{
	ActionComponent = NewActionComponent;
}


void USAction::StartAction_Implementation(AActor* Instigator)
{
	if (!ensure(ActionComponent)) return;
	//LogOnScreen(Instigator, FString::Printf(TEXT("Start Action: %s"), *ActionName.ToString()), FColor::Green, 2.f);
	USActionComponent* ActionComp = GetOwningComponent();

	ActionComp->ActiveGameplayTags.AppendTags(GrantedTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	//LogOnScreen(Instigator, FString::Printf(TEXT("Stop Action: %s"), *ActionName.ToString()), FColor::Red, 2.f);
	//ensureAlways(bIsRunning);
	USActionComponent* ActionComp = GetOwningComponent();

	ActionComp->ActiveGameplayTags.RemoveTags(GrantedTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

//判断当前动作是否包含阻止动作的标签，如果包含则返回false，否则返回true。
bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (RepData.bIsRunning)//避免重复启动同一个动作
	{
		return false;
	}
	USActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}


UWorld* USAction::GetWorld() const
{

	if (ActionComponent)
	{
		return ActionComponent->GetWorld();
	}

	if (const UObject* Outer = GetOuter())
	{
		return Outer->GetWorld();
	}
	
	return nullptr;
}

void USAction::OnRep_RepData()
{
	if (!ActionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActionComponent is null on client, delaying start..."));
		return; // 延迟处理，可选加队列或延迟计时处理
	}

	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}



void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComponent);
}
