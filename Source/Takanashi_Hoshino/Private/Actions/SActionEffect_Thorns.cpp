// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionEffect_Thorns.h"

#include "Actor/SGameplayFunctionLibrary.h"
#include "Component/SAttributeComponent.h"


USActionEffect_Thorns::USActionEffect_Thorns()
{
	ReflectFraction = 0.2f; // 默认反射伤害的比例为25%
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributeComponent* AttributeComp =USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

	if (AttributeComp)
	{
		// 设置属性组件的反射伤害比例
		AttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

	if (AttributeComp)
	{
		// 移除属性组件的反射伤害比例
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{

	AActor* Owner = GetOwningComponent()->GetOwner();

	if (Delta < 0.0f && InstigatorActor != Owner )
	{
		int32 ReflectAmount = FMath::RoundToInt(Delta * ReflectFraction); //将浮点数四舍五入转换为最近的整数

		if (ReflectAmount==0)
		{
			return;
		}

		ReflectAmount=FMath::Abs(ReflectAmount);
		USGameplayFunctionLibrary::ApplyDamage(Owner, InstigatorActor, ReflectAmount);
	}
}
