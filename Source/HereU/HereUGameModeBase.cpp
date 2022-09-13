// Copyright Epic Games, Inc. All Rights Reserved.


#include "HereUGameModeBase.h"

#include "Characters/HereUCharacter_Base.h"
#include "Characters/HereUPlayerController.h"

AHereUGameModeBase::AHereUGameModeBase()
{
	DefaultPawnClass = AHereUCharacter_Base::StaticClass();
	PlayerControllerClass = AHereUPlayerController::StaticClass();
}
