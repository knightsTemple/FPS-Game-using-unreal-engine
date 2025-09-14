// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MyASPowerup_HealthPotion.h"

#include "Character/SPlayerState.h"
#include "Component/SAttributeComponent.h"


class USAttributeComponent;

AMyASPowerup_HealthPotion::AMyASPowerup_HealthPotion()
{
	// 创建 MeshComp 并设置默认网格
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComp->SetupAttachment(RootComponent);
	CreditCost=50;
}

void AMyASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);

	if (!InstigatorPawn)
	{
		return;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (AttributeComp&&!AttributeComp->IsFullHealth())
	{
		// if (AttributeComp->ApplyHealthChange(this,AttributeComp->GetHealthMax()))
		// {
		// 	HideAndCooldownPowerup();
		// }
		if (ASPlayerState*PS=InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if (PS->RemoveCredits(CreditCost)&&AttributeComp->ApplyHealthChange(this,AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
	}
}
