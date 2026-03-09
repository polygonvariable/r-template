// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/TradeCollectionUI.h"

// Generated Headers
#include "ShopCollectionUI.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UShopCollectionUI : public UTradeCollectionUI
{

	GENERATED_BODY()

public:

	// ~ UTradeCollectionUI
	virtual void DisplayEntries() override;
	// ~ End of UTradeCollectionUI

};

