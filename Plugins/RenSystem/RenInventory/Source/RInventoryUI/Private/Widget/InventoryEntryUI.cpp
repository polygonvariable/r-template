// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryEntryUI.h"

// Engine Headers

#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Definition/InventoryPrimaryAsset.h"
#include "InventoryEntry.h"




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

