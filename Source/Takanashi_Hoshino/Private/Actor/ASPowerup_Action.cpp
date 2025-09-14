// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ASPowerup_Action.h"

#include "Actions/SAction.h"

AASPowerup_Action::AASPowerup_Action()
{
}

void AASPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	USActionComponent* ActionComp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));

	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant))
		{
			return; //如果已经拥有该动作，则不再添加
		}

		ActionComp->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPowerup();
	}
}
