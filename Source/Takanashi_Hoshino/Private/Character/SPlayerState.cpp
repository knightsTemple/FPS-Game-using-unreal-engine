#include "Character/SPlayerState.h"

#include "Character/SSaveGame.h"
#include "Net/UnrealNetwork.h"

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

ASPlayerState::ASPlayerState()
{
	SetReplicates(true); // Enable replication for this PlayerState
}

void ASPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta>0.0f))
	{
		return;
	}
	Credits += Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta>0.0f))
	{
		return false;
	}
	if (ensure(Credits<Delta))
	{
		return false;
	}
	Credits -= Delta;
	OnCreditsChanged.Broadcast(this, Credits, -Delta);
	return true;
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		//Credits = SaveObject->Credits;
		AddCredits(SaveObject->Credits);
	}
	
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPlayerState, Credits);
}