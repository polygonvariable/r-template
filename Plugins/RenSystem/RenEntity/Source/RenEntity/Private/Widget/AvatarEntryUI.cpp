// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarEntryUI.h"

// Engine Headers
#include "Engine/AssetManager.h"

#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreEntity/Public/AvatarAsset.h"
#include "RenEntity/Public/Widget/AvatarEntry.h"
#include "RenEntity/Public/EntityPrimaryAsset.h"



void UAvatarEntryUI::SetPrimaryDetails(const UAvatarAsset* Asset)
{
	if (!IsValid(Asset))
	{
		EntryName->SetText(FText::GetEmpty());
		EntryIcon->SetBrushFromSoftTexture(nullptr);

		return;
	}

	if (EntryName)
	{
		EntryName->SetText(Asset->DisplayName);
	}
	if (EntryIcon)
	{
		EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
	}
}

void UAvatarEntryUI::SetSecondaryDetails(const UAvatarAsset* Asset, const FAvatarRecord& Record)
{
}

void UAvatarEntryUI::ResetDetails()
{
	CancelLatentFetch();

	SetPrimaryDetails(nullptr);
}

void UAvatarEntryUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	CancelLatentFetch();

	UAvatarEntry* Entry = Cast<UAvatarEntry>(ListItemObject);
	InitializeDetails(Entry);
}

void UAvatarEntryUI::NativeOnItemSelectionChanged(bool bSelected)
{

}
