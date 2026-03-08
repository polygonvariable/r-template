// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryEntryUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Asset/RPrimaryDataAsset.h"
#include "Widget/InventoryEntry.h"



void UInventoryEntryUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(Asset);
	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	if (!IsValid(InventoryAsset) || !IsValid(InventoryEntry))
	{
		return;
	}

	EntryName->SetText(InventoryAsset->DisplayName);
	EntryIcon->SetBrushFromSoftTexture(InventoryAsset->Icon);
	EntryQuantity->SetText(FText::AsNumber(InventoryEntry->Quantity));
}

