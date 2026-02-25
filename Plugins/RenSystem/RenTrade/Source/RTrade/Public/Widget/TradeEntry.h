// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AssetDetail.h"
#include "Widget/AssetEntry.h"

// Generated Headers
#include "TradeEntry.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UTradeEntry : public UAssetEntry
{

	GENERATED_BODY()

public:

	FAssetDetail_Unique TradeItem;
	FPrimaryAssetId CostAssetId;
	FAssetDetail_Unique CostItem;

};

