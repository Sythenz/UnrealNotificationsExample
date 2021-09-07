// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NotificationsExample.h"
#include "NotificationsExampleStyle.h"
#include "NotificationsExampleCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "SNotificationButtonsWidget.h"

static const FName NotificationsExampleTabName("NotificationsExample");

#define LOCTEXT_NAMESPACE "FNotificationsExampleModule"

void FNotificationsExampleModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FNotificationsExampleStyle::Initialize();
	FNotificationsExampleStyle::ReloadTextures();

	FNotificationsExampleCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FNotificationsExampleCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FNotificationsExampleModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FNotificationsExampleModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FNotificationsExampleModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(NotificationsExampleTabName, FOnSpawnTab::CreateRaw(this, &FNotificationsExampleModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FNotificationsExampleTabTitle", "NotificationsExample"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FNotificationsExampleModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FNotificationsExampleStyle::Shutdown();

	FNotificationsExampleCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(NotificationsExampleTabName);
}

TSharedRef<SDockTab> FNotificationsExampleModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			[
				SNew(SNotificationButtonsWidget)
			]
		];
}

void FNotificationsExampleModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(NotificationsExampleTabName);
}

void FNotificationsExampleModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FNotificationsExampleCommands::Get().OpenPluginWindow);
}

void FNotificationsExampleModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FNotificationsExampleCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNotificationsExampleModule, NotificationsExample)