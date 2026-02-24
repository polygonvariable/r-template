// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/ShopAsset.h"

// Engine Headers

// Project Headers



FPrimaryAssetId UShopAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Asset.Shop"), GetFName());
}

