// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * A CompoundWidget is the base from which most non-primitive widgets should be built.
 * CompoundWidgets have a protected member named ChildSlot.
 */
class SNotificationButtonsWidget : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SNotificationButtonsWidget)
	{}

	SLATE_END_ARGS()

	/**
	* Construct this widget. Called by the SNew() Slate macro.
	*
	* @param  InArgs	Declaration used by the SNew() macro to construct this widget
	*/
	void Construct(const FArguments& InArgs);

	/*
		Part 1
	*/

	//Spawns a simple notification
	FReply SpawnNotification();

	/*
		Part 2
	*/

	//Spawns a notification with a URL link at the bottom right.
	FReply SpawnNotificationWithLink();
	
	/*
		Part 3
	*/

	//Spawns a notification with a Yes/No option.
	FReply SpawnChoiceNotification();

	//Clicked states for the Choice Notification
	void AcceptClicked();
	void DismissClicked();

private:

	/* Used to reference to the Yes/No notification in the Clicked methods */
	TWeakPtr<SNotificationItem> NotificationPtr;
};
