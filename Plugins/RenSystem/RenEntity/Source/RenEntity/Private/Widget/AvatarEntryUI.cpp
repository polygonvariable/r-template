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



bool UAvatarEntryUI::IsValidAssetId(const FPrimaryAssetId& AssetId) const
{
	return EntityPrimaryAsset::IsValid(AssetId);
}

void UAvatarEntryUI::SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
	const UAvatarAsset* AvatarAsset = Cast<UAvatarAsset>(Asset);
	if (!IsValid(AvatarAsset))
	{
		EntryName->SetText(FText::GetEmpty());
		EntryIcon->SetBrushFromSoftTexture(nullptr);

		return;
	}

	if (EntryName)
	{
		EntryName->SetText(AvatarAsset->DisplayName);
	}
	if (EntryIcon)
	{
		EntryIcon->SetBrushFromSoftTexture(AvatarAsset->Icon);
	}
}

