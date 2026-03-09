// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ShopCollectionUI.h"

// Engine Headers

// Project Headers
#include "Asset/ShopAsset.h"
#include "Subsystem/ShopSubsystem.h"
#include "Widget/TradeEntry.h"



void UShopCollectionUI::DisplayEntries()
{
	UShopSubsystem* ShopSubsystem = UShopSubsystem::Get(GetGameInstance());
	if (!IsValid(ShopSubsystem))
	{
		return;
	}

	const UShopAsset* ShopAsset = Cast<const UShopAsset>(TradeAsset);
	if (!IsValid(ShopAsset))
	{
		return;
	}

	ShopSubsystem->QueryItems(ShopAsset, TradeCollectionId,
		[this](const FPrimaryAssetId& ItemAssetId, const FAssetDetail_Trade& ItemDetail) {

			UTradeEntry* Entry = GetEntryFromPool<UTradeEntry>();
			if (IsValid(Entry))
			{
				Entry->TradeDetail = ItemDetail;
				AddEntry(ItemAssetId, Entry);
			}

		}
	);
}

