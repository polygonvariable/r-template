// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarViewUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenEntity/Public/Widget/AvatarCollectionUI.h"
#include "RenEntity/Public/Widget/AvatarDetailUI.h"



void UAvatarViewUI::InitializeDetails(const UAvatarEntry* Entry)
{
	if (IsValid(EntryDetail))
	{
		EntryDetail->InitializeDetails(Entry);
	}
}

void UAvatarViewUI::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UAvatarViewUI::NativeConstruct()
{
	if (EntryCollection)
	{
		EntryCollection->OnEntrySelected.RemoveAll(this);
		EntryCollection->OnEntrySelected.AddUObject(this, &UAvatarViewUI::InitializeDetails);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.RemoveAll(this);
		CloseButton->OnClicked.AddDynamic(this, &UAvatarViewUI::CloseWidget);
	}

	UUserWidget::NativeConstruct();
}

void UAvatarViewUI::NativeDestruct()
{
	CancelLatentFetch();
	UUserWidget::NativeDestruct();
}

