// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/Category/CraftableAsset.h"

// Engine Headers

// Project Headers
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"



const UAssetCollection* UCraftableAsset::GetCraftingMaterial() const
{
	if (!IsValid(CraftingItems))
	{
		return nullptr;
	}
	return CraftingItems->GetCollectionRule<UAssetCollection>();
}

const UAssetCollection* UCraftableAsset::GetCraftingMaterial(const FInstancedStruct& Context) const
{
	if (!IsValid(CraftingItems))
	{
		return nullptr;
	}
	return CraftingItems->GetCollectionRule<UAssetCollection>(Context);
}

