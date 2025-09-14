// Copyright Epic Games, Inc. All Rights Reserved.

#include "Takanashi_HoshinoGameMode.h"
#include "Takanashi_HoshinoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATakanashi_HoshinoGameMode::ATakanashi_HoshinoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
