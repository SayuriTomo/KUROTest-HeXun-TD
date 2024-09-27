// Copyright Epic Games, Inc. All Rights Reserved.

#include "KUROTest_HeXun_TDGameMode.h"
#include "KUROTest_HeXun_TDCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKUROTest_HeXun_TDGameMode::AKUROTest_HeXun_TDGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
