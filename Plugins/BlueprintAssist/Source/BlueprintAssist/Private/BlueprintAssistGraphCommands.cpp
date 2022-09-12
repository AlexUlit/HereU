// Copyright 2021 fpwong. All Rights Reserved.

#include "BlueprintAssistGraphCommands.h"

#define LOCTEXT_NAMESPACE "BlueprintAssist"

void FBAGraphCommands::RegisterCommands()
{
	UI_COMMAND(
		GenerateGetter,
		"Generate Getter",
		"Generates a getter function for the variable",
		EUserInterfaceActionType::Button,
		FInputChord());

	UI_COMMAND(
		GenerateSetter,
		"Generate Setter",
		"Generates a setter function for the variable",
		EUserInterfaceActionType::Button,
		FInputChord());

	UI_COMMAND(
		GenerateGetterAndSetter,
		"Generate Getter and Setter",
		"Generates a getter and setter function for the variable",
		EUserInterfaceActionType::Button,
		FInputChord());

	UI_COMMAND(
		ConvertGetToSet,
		"Convert Get to Set",
		"Converts this node from a Get to a Set",
		EUserInterfaceActionType::Button,
		FInputChord());

	UI_COMMAND(
		ConvertSetToGet,
		"Convert Set to Get",
		"Converts this node from a Set to a Get",
		EUserInterfaceActionType::Button,
		FInputChord());
}

#undef LOCTEXT_NAMESPACE
