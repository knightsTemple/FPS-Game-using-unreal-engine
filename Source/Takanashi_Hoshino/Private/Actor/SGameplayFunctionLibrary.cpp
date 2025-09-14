// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SGameplayFunctionLibrary.h"

#include "Component/SAttributeComponent.h"

class USAttributeComponent;

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamagedCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamagedCauser, DamageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent&&HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector Direction=HitResult.TraceEnd-HitResult.TraceStart;
			Direction.Normalize();
			HitComponent->AddImpulseAtLocation(Direction*300000, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
