// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/TradeCollectionUI.h"

// Generated Headers
#include "ShopCollectionUI.generated.h"

// Forward Declarations
class UAssetCollection;
class URPrimaryDataAsset;



/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UShopCollectionUI : public UTradeCollectionUI
{

	GENERATED_BODY()

protected:

	// ~ UTradeCollectionUI
	virtual const UAssetCollection* GetAssetCollection(const URPrimaryDataAsset* Asset) const override;
	// ~ End of UTradeCollectionUI

};

