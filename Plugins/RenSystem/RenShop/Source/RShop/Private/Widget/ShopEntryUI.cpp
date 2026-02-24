// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ShopEntryUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Management/AssetCollection.h"
#include "Widget/ShopEntry.h"



void UShopEntryUI::SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UShopEntry* ShopEntry = Cast<UShopEntry>(Entry);
	if (!IsValid(ShopEntry))
	{
		return;
	}

	if (IsValid(EntryName)) EntryName->SetText(Asset->DisplayName);
	if (IsValid(EntryIcon)) EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
	if (IsValid(CostQuantity)) CostQuantity->SetText(FText::FromString(FString::FromInt(ShopEntry->ShopItem.Quantity)));
}

