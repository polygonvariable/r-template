// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ShopCollectionUI.h"

// Engine Headers

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Interface/ShopProviderInterface.h"



const UAssetCollection* UShopCollectionUI::GetAssetCollection(const URPrimaryDataAsset* Asset) const
{
	const IShopProviderInterface* ShopProvider = Cast<IShopProviderInterface>(Asset);
	if (!ShopProvider)
	{
		return nullptr;
	}
	return ShopProvider->GetPurchaseCost();
}

