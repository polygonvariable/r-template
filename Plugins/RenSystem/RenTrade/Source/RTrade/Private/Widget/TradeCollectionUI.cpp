// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeCollectionUI.h"

// Engine Headers

// Project Headers
#include "Asset/TradeAsset.h"
#include "Definition/AssetRuleDefinition.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollectionTrade.h"
#include "Manager/RAssetManager.inl"
#include "Widget/TradeEntry.h"



void UTradeCollectionUI::SetTradeAsset(const UTradeAsset* Asset)
{
	TradeAsset = Asset;
}

void UTradeCollectionUI::SetTradeCollectionId(FGuid CollectionId)
{
	TradeCollectionId = CollectionId;
}

