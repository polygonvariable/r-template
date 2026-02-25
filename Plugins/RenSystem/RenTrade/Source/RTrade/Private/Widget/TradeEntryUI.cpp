// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeEntryUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Widget/TradeEntry.h"



void UTradeEntryUI::SetPrimaryDetails(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UTradeEntry* ShopEntry = Cast<UTradeEntry>(Entry);
	if (!IsValid(ShopEntry))
	{
		return;
	}

	EntryName->SetText(Asset->DisplayName);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
	TradeQuantity->SetText(FText::FromString(FString::FromInt(ShopEntry->TradeItem.Quantity)));
}

