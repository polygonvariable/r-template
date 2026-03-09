// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetCollectionUI.h"

// Generated Headers
#include "TradeCollectionUI.generated.h"

// Module Macros
#define RSYSTEM_API RTRADE_API

// Forward Declarations
class UTradeAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI, NotBlueprintable)
class UTradeCollectionUI : public UAssetCollectionUI
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<const UTradeAsset> TradeAsset = nullptr;

	UPROPERTY()
	FGuid TradeCollectionId;

};



// Module Macros
#undef RSYSTEM_API

