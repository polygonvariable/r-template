// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeEntryUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/RPrimaryDataAsset.h"
#include "Widget/TradeEntry.h"



void UTradeEntryUI::SetPrimaryDetail(const UAssetEntry* Entry, const URPrimaryDataAsset* Asset)
{
	const UTradeEntry* ShopEntry = Cast<UTradeEntry>(Entry);
	if (!IsValid(ShopEntry))
	{
		return;
	}

	int Quantity = ShopEntry->TradeDetail.Quantity;
	int Quota = ShopEntry->TradeDetail.Quota;

	EntryName->SetText(Asset->DisplayName);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
	TradeQuantity->SetText(FText::FromString(FString::FromInt(Quantity)));
	TradeQuota->SetText(FText::FromString(FString::FromInt(Quota)));
}

