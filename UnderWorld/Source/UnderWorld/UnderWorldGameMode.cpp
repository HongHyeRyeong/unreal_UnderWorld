// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnderWorldGameMode.h"
#include "UnderWorldPlayerController.h"
#include "UnderWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnderWorldGameMode::AUnderWorldGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUnderWorldPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}