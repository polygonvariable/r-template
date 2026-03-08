// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/TradeAsset.h"

// Generated Headers
#include "ShopAsset.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UShopAsset : public UTradeAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
    FTimespan RefreshTime;

};

