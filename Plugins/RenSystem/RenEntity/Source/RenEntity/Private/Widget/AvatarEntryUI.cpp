// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarEntryUI.h"

// Engine Headers
#include "Engine/AssetManager.h"

#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"

#include "Asset/AvatarAsset.h"
#include "Widget/AvatarEntry.h"
#include "EntityPrimaryAsset.h"



bool UAvatarEntryUI::IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const
{
	return EntityPrimaryAsset::IsValid(AssetId);
}

void UAvatarEntryUI::SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
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

