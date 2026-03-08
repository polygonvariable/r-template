// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CraftCollectionUI.h"

// Engine Headers

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Interface/CraftProviderInterface.h"
#include "Asset/TradeAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollectionUnique.h"
#include "Manager/RAssetManager.inl"
#include "Widget/TradeEntry.h"



//const UAssetCollection* UCraftCollectionUI::GetAssetCollection(const URPrimaryDataAsset* Asset) const
//{
//	const ICraftProviderInterface* CraftProvider = Cast<ICraftProviderInterface>(Asset);
//	if (!CraftProvider)
//	{
//		return nullptr;
//	}
//	return CraftProvider->GetCraftingMaterial();
//}

