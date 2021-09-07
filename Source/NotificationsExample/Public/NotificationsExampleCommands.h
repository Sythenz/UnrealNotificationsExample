// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "NotificationsExampleStyle.h"

class FNotificationsExampleCommands : public TCommands<FNotificationsExampleCommands>
{
public:

	FNotificationsExampleCommands()
		: TCommands<FNotificationsExampleCommands>(TEXT("NotificationsExample"), NSLOCTEXT("Contexts", "NotificationsExample", "NotificationsExample Plugin"), NAME_None, FNotificationsExampleStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};