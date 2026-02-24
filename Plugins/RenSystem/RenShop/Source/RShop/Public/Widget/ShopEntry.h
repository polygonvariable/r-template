// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Management/AssetCollection.h"
#include "Widget/AssetEntry.h"

// Generated Headers
#include "ShopEntry.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UShopEntry : public UAssetEntry
{

	GENERATED_BODY()

public:

	FAssetDetail_Unique ShopItem;
	FPrimaryAssetId CostAssetId;
	FAssetDetail_Unique CostItem;

};

