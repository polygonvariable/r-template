// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/InventoryAsset.h"

// Engine Headers

// Project Headers



FPrimaryAssetType UInventoryAsset::GetPrimaryAssetType()
{
	return TEXT("Asset.Inventory");
}

FPrimaryAssetId UInventoryAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UInventoryAsset::GetPrimaryAssetType(), GetFName());
}

const UAssetCollection* UInventoryAsset::GetBreakdownAssets(const FGuid& InId) const
{
	return nullptr;
}

const UAssetCollection* UInventoryAsset::GetRebuildAssets(const FGuid& InId) const
{
	return nullptr;
}

