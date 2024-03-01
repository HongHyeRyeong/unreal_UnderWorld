// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnderWorldGameMode.h"
#include "UnderWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnderWorldGameMode::AUnderWorldGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
