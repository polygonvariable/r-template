// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryEntryUI.h"

// Engine Headers
#include "Engine/AssetManager.h"

#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "InventoryDefinition.h"
#include "InventoryEntry.h"

#include "InventoryPrimaryAsset.h"




bool UInventoryEntryUI::IsPrimaryAssetIdValid(const FPrimaryAssetId& AssetId) const
{
	return InventoryPrimaryAsset::IsValid(AssetId);
}

void UInventoryEntryUI::SetPrimaryDetails(const UCatalogEntry* Entry, const UPrimaryDataAsset* Asset)
{
	const UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(Asset);
	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	if (!IsValid(InventoryAsset) || !IsValid(InventoryEntry))
	{
		return;
	}

	if (IsValid(EntryName)) EntryName->SetText(InventoryAsset->DisplayName);
	if (IsValid(EntryIcon)) EntryIcon->SetBrushFromSoftTexture(InventoryAsset->Icon);
	if (IsValid(EntryQuantity)) EntryQuantity->SetText(FText::AsNumber(InventoryEntry->Quantity));
}

