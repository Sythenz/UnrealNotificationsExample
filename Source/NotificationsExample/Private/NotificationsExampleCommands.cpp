// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NotificationsExampleCommands.h"

#define LOCTEXT_NAMESPACE "FNotificationsExampleModule"

void FNotificationsExampleCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "NotificationsExample", "Bring up NotificationsExample window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
