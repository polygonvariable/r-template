// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AssetDetail_Trade.h"
#include "Widget/AssetEntry.h"

// Generated Headers
#include "TradeEntry.generated.h"

// Module Macros
#define RSYSTEM_API RTRADE_API

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UTradeEntry : public UAssetEntry
{

	GENERATED_BODY()

public:

	FAssetDetail_Trade TradeDetail;

	// ~ UAssetEntry
	RSYSTEM_API virtual void ResetData() override
	{
		TradeDetail.Reset();
		Super::ResetData();
	}
	// ~ End of UAssetEntry

};



// Module Macros
#undef RSYSTEM_API

