// Copyright Epic Games, Inc. All Rights Reserved.


#include "HereUGameModeBase.h"

#include "Characters/ControllerProtagonist.h"
#include "Characters/HereUCharacter_Base.h"
#include "Characters/HereUPlayerController_Base.h"

AHereUGameModeBase::AHereUGameModeBase()
{
	DefaultPawnClass = AHereUCharacter_Base::StaticClass();
	PlayerControllerClass = AControllerProtagonist::StaticClass();
}
