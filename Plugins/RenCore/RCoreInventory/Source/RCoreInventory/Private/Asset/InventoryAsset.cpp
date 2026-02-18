// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/InventoryAsset.h"

// Engine Headers

// Project Headers



FPrimaryAssetType UInventoryAsset::GetPrimaryAssetType()
{
	return TEXT("Inventory");
}

FPrimaryAssetId UInventoryAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UInventoryAsset::GetPrimaryAssetType(), GetFName());
}

const UAssetCollection* UInventoryAsset::GetBreakdownAssets(const FGameplayTagContainer& InTags) const
{
	return nullptr;
}

const UAssetCollection* UInventoryAsset::GetRebuildAssets(const FGameplayTagContainer& InTags) const
{
	return nullptr;
}

