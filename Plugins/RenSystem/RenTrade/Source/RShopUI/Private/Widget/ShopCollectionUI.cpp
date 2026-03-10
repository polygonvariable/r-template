// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ShopCollectionUI.h"

// Engine Headers

// Project Headers
#include "Storage/ShopStorage.h"
#include "Subsystem/ShopSubsystem.h"
#include "Widget/TradeEntry.h"



void UShopCollectionUI::DisplayEntries()
{
	if (!IsValid(ShopSubsystem))
	{
		return;
	}

	ShopSubsystem->QueryItems(TradeAsset, TradeCollectionId,
		[this](const FPrimaryAssetId& ItemAssetId, const FAssetDetail_Trade& ItemDetail)
		{
			UTradeEntry* Entry = GetEntryFromPool<UTradeEntry>();
			if (IsValid(Entry))
			{
				Entry->TradeDetail = ItemDetail;
				AddEntry(ItemAssetId, Entry);
			}
		}
	);
}

void UShopCollectionUI::NativeConstruct()
{
	ShopSubsystem = UShopSubsystem::Get(GetGameInstance());
	if (IsValid(ShopSubsystem))
	{
		UShopStorage* ShopStorage = ShopSubsystem->GetShopStorage();
		if (IsValid(ShopStorage))
		{
			ShopStorage->OnShopUpdated.AddUObject(this, &UShopCollectionUI::RefreshEntries);
		}
	}

	Super::NativeConstruct();
}

void UShopCollectionUI::NativeDestruct()
{
	if (IsValid(ShopSubsystem))
	{
		UShopStorage* ShopStorage = ShopSubsystem->GetShopStorage();
		if (IsValid(ShopStorage))
		{
			ShopStorage->OnShopUpdated.RemoveAll(this);
		}
	}
	ShopSubsystem = nullptr;

	Super::NativeDestruct();
}

