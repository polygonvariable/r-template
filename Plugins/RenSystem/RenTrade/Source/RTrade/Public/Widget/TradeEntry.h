// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AssetDetailTrade.h"
#include "Widget/AssetEntry.h"

// Generated Headers
#include "TradeEntry.generated.h"

// Forward Declarations
class UAssetCollection;



/**
 *
 */
UCLASS(MinimalAPI)
class UTradeEntry : public UAssetEntry
{

	GENERATED_BODY()

public:

	//FGuid MaterialCollectionId;
	FAssetDetail_Trade TradeDetail;

	RTRADE_API virtual void ResetData() override
	{
		//MaterialCollectionId.Invalidate();
		TradeDetail.Reset();
		Super::ResetData();
	}

};

