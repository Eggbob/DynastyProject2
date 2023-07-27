// Copyright Epic Games, Inc. All Rights Reserved.

#include "DynastyProjectGameMode.h"
#include "DynastyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADynastyProjectGameMode::ADynastyProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
}
