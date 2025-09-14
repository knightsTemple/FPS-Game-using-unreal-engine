// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionEffect.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true; //默认自动启动
}
void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	//超过Buff的持续时间后，停止Buff
	if (Duration>0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, FName("StopAction"), Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	//如果有周期性的效果，则每隔一段时间执行一次
	if (Period>0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, FName("ExecutePeriodicEffect"), Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle)<KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator); //确保在停止动作时执行一次周期性效果
	}
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	USActionComponent* Comp=GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}



void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}
