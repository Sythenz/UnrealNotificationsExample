// Fill out your copyright notice in the Description page of Project Settings.

#include "SNotificationButtonsWidget.h"
#include <SlateOptMacros.h>
#include "Widgets/Input/SButton.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/SBoxPanel.h"

#define LOCTEXT_NAMESPACE "SNotificationButtonsWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SNotificationButtonsWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		//Part 1
		
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		[
			SNew(SButton)
			.Text(LOCTEXT("SpawnNotification", "Spawn Notification"))
			.ToolTipText(LOCTEXT("SpawnNotification_Tooltip", "Creates a super cool Notification!"))
			.OnClicked(this, &SNotificationButtonsWidget::SpawnNotification)	
		]
		
		//Part 2
		
		+SHorizontalBox::Slot()
		[
			SNew(SButton)
			.Text(LOCTEXT("SpawnNotificationWithLink", "Spawn Notification With Link"))
			.ToolTipText(LOCTEXT("SpawnNotificationWithLink_Tooltip", "Creates a Notification with a link!"))
			.OnClicked(this, &SNotificationButtonsWidget::SpawnNotificationWithLink)
		]

		//Part 3
		
		+SHorizontalBox::Slot()
		[
			SNew(SButton)
			.Text(LOCTEXT("SpawnNotificationWithChoice", "Spawn A Choice Notification"))
			.ToolTipText(LOCTEXT("SpawnNotificationWithChoice_Tooltip", "Creates a notification with a Yes/No Choice!"))
			.OnClicked(this, &SNotificationButtonsWidget::SpawnChoiceNotification)	
		]
	];
}

/*
	Part 1 - Basic Notifications
*/

FReply SNotificationButtonsWidget::SpawnNotification()
{
	FNotificationInfo Info(LOCTEXT("SpawnNotification_Notification", "This is a notification."));
	Info.ExpireDuration = 5.0f;
	FSlateNotificationManager::Get().AddNotification(Info);

	return FReply::Handled();
}

/*
	Part 2 - Notification styling and Hyperlinks
*/

FReply SNotificationButtonsWidget::SpawnNotificationWithLink()
{
	//Simple declaration of a notification. Passed to the SlateNotificationManager through Add Notification.
	FNotificationInfo Info(LOCTEXT("SpawnNotificationWithLink_Notification", "Unable to do a thing! Please check the documentation."));
	Info.ExpireDuration = 5.0f;
	
	//To use an icon in your notifications, add EditorStyle to your Build.cs under PrivateDependencyModuleNames
	Info.bUseSuccessFailIcons = true;
	Info.Image = FEditorStyle::GetBrush(TEXT("MessageLog.Note"));

	/* 
		Create a Hyperlink that appears at the bottom right of the notification.
		A Hyperlink itself can call a function, in this case we create a small lambda to launch a URL.
		but you could call other functions here.
	*/
	Info.Hyperlink = FSimpleDelegate::CreateLambda([this]() {
		const FString DocsURL = TEXT("https://docs.unrealengine.com/");
		FPlatformProcess::LaunchURL(*DocsURL, nullptr, nullptr);
	});

	Info.HyperlinkText = LOCTEXT("GoToDocs", "Go to Documentation...");

	FSlateNotificationManager::Get().AddNotification(Info);

	return FReply::Handled();
}

/*
	Part 3 - Notifications with button choices and their delegates
*/

FReply SNotificationButtonsWidget::SpawnChoiceNotification()
{
	/*
	 In our header we have defined an empty NotificationPtr.
	 A pin here converts a weak pointer to a shared one if it hasn't expired.
	 We assign to this NotificationPtr when we create our Notification later.
	*/
	TSharedPtr<SNotificationItem> NotificationPin = NotificationPtr.Pin();

	//We need to validate the new shared pointer because it could have expired.
	if (NotificationPin.IsValid())
	{
		//Do nothing if the pointer was incorrectly handled for some reason.
		return FReply::Handled();
	}

	//This is an alternate way to pass in the Notification Title when declaring the FNotificationInfo
	const FText NotificationText = LOCTEXT("SlateGuideCreateNotificationButtons", "Restart The Editor?");
	FNotificationInfo Info(NotificationText);

	// Add the buttons with text, tooltip and callback
	Info.ButtonDetails.Add(FNotificationButtonInfo(
		LOCTEXT("RestartNow", "Restart"),
		LOCTEXT("RestartNowToolTip", "Restarts the Editor"),
		FSimpleDelegate::CreateRaw(this, &SNotificationButtonsWidget::AcceptClicked))
	);
	Info.ButtonDetails.Add(FNotificationButtonInfo(
		LOCTEXT("RestartLater", "Cancel"),
		LOCTEXT("RestartLaterToolTip", "Cancels the Editor Restart"),
		FSimpleDelegate::CreateRaw(this, &SNotificationButtonsWidget::DismissClicked))
	);

	/*
	  Prevents the notification from expiring and keeps it on screen until a button
	  has been pressed.
	*/
	Info.bFireAndForget = false;

	//Some additional styling information
	Info.WidthOverride = 200.0f;
	Info.bUseLargeFont = false;
	Info.bUseThrobber = false;
	Info.bUseSuccessFailIcons = false;
	//Display a different info icon than default warning
	Info.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));

	/*
	  Launch notification - This is where we store a pointer to the notification,
	  the reason we do this is because we need to continue to access this in the button
	  states below - and is the reason we've defined it in the header and not here in
	  the method.
	*/
	NotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);

	if (NotificationPin.IsValid())
	{
		//Setting the completed state of the Notification Pin is used for handling
		//the transition animations and fading depending on the fail state and clean up.
		NotificationPin->SetCompletionState(SNotificationItem::CS_Pending);
	}

	return FReply::Handled();
}

void SNotificationButtonsWidget::AcceptClicked()
{
	TSharedPtr<SNotificationItem> NotificationPin = NotificationPtr.Pin();
	if (NotificationPin.IsValid())
	{
		//Set new text on our existing notification
		NotificationPin->SetText(LOCTEXT("RestartingNow", "You can do a thing here, SNotificationsWidget::OnNotificationAcceptClicked()"));
		
		NotificationPin->SetCompletionState(SNotificationItem::CS_Success);

		//Inform our notification to fade out
		NotificationPin->ExpireAndFadeout();
		
		//Clean up
		NotificationPtr.Reset();
	}

	//Do thing here...
}

void SNotificationButtonsWidget::DismissClicked()
{
	TSharedPtr<SNotificationItem> NotificationPin = NotificationPtr.Pin();
	if (NotificationPin.IsValid())
	{
		NotificationPin->SetText(LOCTEXT("RestartDismissed", "Dismissed..."));
		NotificationPin->SetCompletionState(SNotificationItem::CS_None);
		NotificationPin->ExpireAndFadeout();
		NotificationPtr.Reset();
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE