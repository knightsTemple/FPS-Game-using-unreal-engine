// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext, const FString& Message, FColor Color = FColor::White, float Duration = 5.f)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* World=WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	FString NetPredix = World->IsNetMode(NM_Client) ? TEXT("[Server] ") : TEXT("[Client] ");
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPredix + Message);
	}
}