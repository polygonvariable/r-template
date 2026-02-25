// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/TradeAsset.h"

// Engine Headers

// Project Headers



FPrimaryAssetId UTradeAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Asset.Trade"), GetFName());
}

